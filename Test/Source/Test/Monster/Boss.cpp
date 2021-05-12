// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "MonsterAIController.h"
#include "../Collision/DragonCollisionManager.h"
#include "../Components/MonsterStatusManager.h"
#include "Components/DecalComponent.h"
#include "../Components/SoundEffectComponent.h"
#include "RockProjectile.h"
#include "MonsterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"


ABoss::ABoss() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MonsterID = 1;

	GetCapsuleComponent()->SetWorldScale3D(FVector::OneVector * 3);

	GetMesh()->SetRelativeTransform(FTransform(FRotator(0, -90, 0).Quaternion(), FVector(0, 0, -80.0f), FVector::OneVector));
	GetMesh()->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BASEMESH(TEXT("SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Skins/Tier2/Elemental/Meshes/Rampage_Elemental.Rampage_Elemental'"));
	if (BASEMESH.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BASEMESH.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/Blueprints/Anim/Dragon/RampageAnimBP.RampageAnimBP_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}


	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel6);
	GetMesh()->SetCollisionProfileName("MonsterHitBox");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->MaxAcceleration = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeed = BasicSpeed;
	GetCharacterMovement()->NavAgentProps.bCanFly = true;
	
	MaterialMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MaterailMesh"));
	MaterialMesh->SetupAttachment(GetMesh());
	MaterialMesh->SetSkeletalMesh(BASEMESH.Object);
	MaterialMesh->SetVisibility(false);
	MaterialMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MaterialMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat::Identity);
	MaterialMesh->SetMasterPoseComponent(GetMesh());
	MaterialMesh->SetHiddenInGame(true);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -95.0f));

	TrailLeftHand = CreateDefaultSubobject<UParticleSystemComponent>("TrailParticle1");
	TrailLeftHand->SetupAttachment(GetMesh());

	TrailRightHand = CreateDefaultSubobject<UParticleSystemComponent>("TrailParticle2");
	TrailRightHand->SetupAttachment(GetMesh());


	AttackRange = 1000.0f;
	RoarRange = 7000.0f;

	BasicSpeed = 400.0f;
	RunSpeed = 1200.0f;
	FlySpeed = 2400.0f;
	GetCharacterMovement()->MaxAcceleration = RunSpeed;

	IsFly = false;

	BrokenState = 0;

}
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	AnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
bool ABoss::MonsterInit(FMonsterStatus status) {

	auto DataTable = LoadObject<UDataTable>(NULL, *(status.PartsDataRef), NULL, LOAD_None, NULL);

	MonsterName = status.MonsterName;
	if (DataTable != nullptr && HitBox !=nullptr) {
		HitBox->InitHitBox(DataTable, GetMesh());
	}
	Status->SetStatus(status.HP, status.Damage);
	Status->DeadDel.AddUObject(this, &ABoss::Dead);

	MonsterController = Cast<AMonsterAIController>(GetController());
	MonsterController->StateChangeDel.AddUObject(this, &ABaseMonster::ChangeMonsterState);
	MonsterController->StateChangeDel.AddUObject(AnimInst, &UMonsterAnimInstance::ChangeMonsterState);
	MonsterController->StateChangeDel.Broadcast(EMonsterState::E_IDLE);

	GetMesh()->SetScalarParameterValueOnMaterials("ScarScale", 0.0f);

	return true;
}
void ABoss::Dead() {

	MonsterController->ChangeMonsterState(EMonsterState::E_DEAD);
}

void ABoss::ChangeMonsterState(EMonsterState state) {
	if (CurrentState == state) return;
	CurrentState = state;
	switch (CurrentState) {
		case EMonsterState::E_IDLE:
			GetCharacterMovement()->MaxWalkSpeed = BasicSpeed;
			break;
		case EMonsterState::E_BATTLE:
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			break;
		case EMonsterState::E_RAGE:
			GetMesh()->SetScalarParameterValueOnMaterials("GlowStart", 1.5f);
			TrailLeftHand->BeginTrails("FX_Trail_L_01","lowerarm_l",ETrailWidthMode::ETrailWidthMode_FromFirst,1.5f);
			TrailRightHand->BeginTrails("FX_Trail_R_01", "lowerarm_r", ETrailWidthMode::ETrailWidthMode_FromFirst, 1.5f);
			break;
		case EMonsterState::E_DEAD:
			if (AnimInst != nullptr) {
				AnimInst->MonsterDead();
			}
			//Todo : Interactive Components On(this component is Get Item)
			GetMesh()->SetCollisionProfileName("Dead");
			break;
	}
}

void ABoss::SetBrokenState(EMonsterPartsType brokenPart) {
	EMonsterBrokenParts part = EMonsterBrokenParts::E_NONE;
	switch (brokenPart)
	{
	case EMonsterPartsType::E_HEAD:
		part = EMonsterBrokenParts::E_HEAD;
		break;
	case EMonsterPartsType::E_BODY:
		part = EMonsterBrokenParts::E_BODY;
		break;
	case EMonsterPartsType::E_LEFTHAND:
		part = EMonsterBrokenParts::E_RIGHTHAND| EMonsterBrokenParts::E_LEFTHAND;
		TESTLOG(Warning, TEXT("Broken %d, BitFlage %d"), BrokenState, (int32)part);
		BrokenState = BrokenState | (int32)part;
		TESTLOG(Warning, TEXT("Broken %d"), BrokenState);
		HitBox->BrokenPart(EMonsterPartsType::E_RIGHTHAND);
		HitBox->BrokenPart(EMonsterPartsType::E_LEFTHAND);
		GetMesh()->SetScalarParameterValueOnMaterials("ScarScale", 10.0f);
		break;
	case EMonsterPartsType::E_RIGHTHAND:
		part = EMonsterBrokenParts::E_RIGHTHAND| EMonsterBrokenParts::E_LEFTHAND;
		TESTLOG(Warning, TEXT("Broken %d, BitFlage %d"),BrokenState,(int32)part);
		BrokenState = BrokenState | (int32)part;
		TESTLOG(Warning, TEXT("Broken %d"), BrokenState);
		HitBox->BrokenPart(EMonsterPartsType::E_RIGHTHAND);
		HitBox->BrokenPart(EMonsterPartsType::E_LEFTHAND);
		GetMesh()->SetScalarParameterValueOnMaterials("ScarScale", 10.0f);
		break;
	case EMonsterPartsType::E_WING:
		part = EMonsterBrokenParts::E_WING;
		break;
	case EMonsterPartsType::E_LEFTLEG:
		part = EMonsterBrokenParts::E_LEFTLEG;
		break;
	case EMonsterPartsType::E_RIGHTLEG:
		part = EMonsterBrokenParts::E_RIGHTLEG;
		break;
	case EMonsterPartsType::E_TAIL:
		part = EMonsterBrokenParts::E_TAIL;
		break;
	}
}


void ABoss::Roar() {

	TArray<FHitResult> DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);

	bool bHit = GetWorld()->SweepMultiByProfile(
		DetectResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		TEXT("MonsterDamage"),
		FCollisionShape::MakeSphere(RoarRange),
		params);

	//Todo : ControllingPawn Play Sound Roar

	bool IsWeak = false;
	int32 OutFinalDamage = 0;

	TArray<AActor*> repeatedActor;

	for (auto DetectCharacter : DetectResult) {

		auto Character = Cast<IDamageInterface>(DetectCharacter.Actor);

		if (Character != nullptr && !repeatedActor.Contains(DetectCharacter.GetActor())) {
			Character->TakeDamageFunc(IsWeak, OutFinalDamage, this, DetectCharacter, 0, EDamageType::E_ROAR, NULL);
			repeatedActor.Add(DetectCharacter.GetActor());
		}
	}
}
ARockProjectile* ABoss::SpawnProjectile() {
	if (ProjectileType == nullptr) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	FRotator ProjectileMuzzleRotation = FRotator::ZeroRotator; //GetMesh()->GetSocketRotation("Muzzle_01");
	FVector ProjectileMuzzleLocation = (GetMesh()->GetSocketLocation("Muzzle_01") + GetMesh()->GetSocketLocation("Muzzle_02")) / 2;
	ARockProjectile* Projectile;

	Projectile = GetWorld()->SpawnActor<ARockProjectile>(ProjectileType, ProjectileMuzzleLocation, ProjectileMuzzleRotation, SpawnParams);
	Projectile->SpawnRock(Status->GetDamage(), 300.0f, this, EDamageType::E_KNOCKBACK, 300.0f);

	return Projectile;
}
void ABoss::ShootProjectile(ARockProjectile* rock) {
	FVector Direction = (MonsterController->GetTarget()->GetActorLocation()) - GetActorLocation();
	Direction.Normalize();
	FVector LaunchDirection = Direction;
	rock->ThrowRock(LaunchDirection);
}
/*
void ABoss::ShootFireBall() {
	if (ProjectileType == nullptr) return;


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	FRotator ProjectileMuzzleRotation = GetMesh()->GetSocketRotation("Mouse");
	FVector ProjectileMuzzleLocation = GetMesh()->GetSocketLocation("Mouse");
	ADragonFireBall* Projectile = FindFireBall();

	if (Projectile!=nullptr) {
		Projectile->SetActorLocationAndRotation(ProjectileMuzzleLocation, ProjectileMuzzleRotation);
	}
	else {
		Projectile = GetWorld()->SpawnActor<ADragonFireBall>(ProjectileType, ProjectileMuzzleLocation, ProjectileMuzzleRotation, SpawnParams);
		FireBalls.Add(Projectile);
	}
	if (IsValid(Projectile))
	{
		FVector Direction = (MonsterController->GetTarget()->GetActorLocation()) - ProjectileMuzzleLocation;
		Direction.Normalize();
		// Set the projectile's initial trajectory.
		FVector LaunchDirection = Direction;
		Projectile->SpawnFireBall(Status->GetDamage(), 300.0f, this, EDamageType::E_KNOCKBACK,300.0f);
		Projectile->ShootDirection(LaunchDirection);

	}
}
ADragonFireBall*  ABoss::FindFireBall() {
	if (FireBalls.Num() <= 0) return nullptr;

	for (auto fireball : FireBalls) {
		if (!fireball->GetIsWorking()) {
			return fireball;
		}
	}
	return nullptr;
}
*/