// Fill out your copyright notice in the Description page of Project Settings.


#include "BossDragon.h"
#include "MonsterAIController.h"
#include "../Collision/DragonCollisionManager.h"
#include "../Components/MonsterStatusManager.h"
#include "Components/DecalComponent.h"
#include "DragonFireBall.h"
#include "MonsterAnimInstance.h"


ABossDragon::ABossDragon() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MonsterID = 1;

	GetMesh()->SetRelativeTransform(FTransform(FRotator(0, -90, 0).Quaternion(), FVector(0, 0, -80.0f), FVector::OneVector));
	GetMesh()->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BASEMESH(TEXT("SkeletalMesh'/Game/QuadrapedCreatures/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON'"));
	if (BASEMESH.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BASEMESH.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/Blueprints/Anim/Dragon/DragonAnimBP.DragonAnimBP_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}

	FString AnimDataPath = FString("DataTable'/Game/DataTable/Monster/DragonAnim.DragonAnim'");
	SetUpAnim(AnimDataPath);


	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel6);
	GetMesh()->SetCollisionProfileName("MonsterHitBox");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->MaxAcceleration = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeed = BasicSpeed;
	GetCharacterMovement()->NavAgentProps.bCanFly = true;
	
	HitBox = CreateDefaultSubobject<UDragonCollisionManager>(TEXT("HITBOX"));
	
	Status = CreateDefaultSubobject<UMonsterStatusManager>(TEXT("STATUS"));

	_Detect = CreateDefaultSubobject<UDetectComponent>(TEXT("DETECTMANAGER"));
	_Detect->SetDetectRange(DetectRange, EDetectCollisionType::E_PLAYER);
	_Detect->GetDetect()->SetupAttachment(GetCapsuleComponent());

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	AttackRange = 300;
	AggroChangeTerm = 15.0f;

	BasicSpeed = 400.0f;
	RunSpeed = 1200.0f;
	FlySpeed = 2400.0f;
	GetCharacterMovement()->MaxAcceleration = RunSpeed;

	CurrentState = EMonsterState::E_CREATE;
	IsFly = false;

	for (int i = 0; i < 5; i++) {
		FName str = FName(TEXT("Wound"), i);
		UDecalComponent* Decal = CreateDefaultSubobject<UDecalComponent>(str);
		Wounds.Add(Decal);
	}
	Wounds[0]->AttachTo(GetMesh(), FName("MOUNTAIN_DRAGON_-Head"));
	Wounds[1]->AttachTo(GetMesh(), FName("MOUNTAIN_DRAGON_-Head"));
	Wounds[2]->AttachTo(GetMesh(), FName("MOUNTAIN_DRAGON_-Tail2"));
	Wounds[3]->AttachTo(GetMesh(), FName("MOUNTAIN_DRAGON_-Tail7"));
	Wounds[4]->AttachTo(GetMesh(), FName("MOUNTAIN_DRAGON_-Tail10"));

}
void ABossDragon::BeginPlay()
{
	Super::BeginPlay();
	IsFly = GetCharacterMovement()->IsFlying();
	for (auto wound : Wounds) {
		wound->SetVisibility(false);
	}
}

void ABossDragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->IsFlying();
}

void ABossDragon::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
bool ABossDragon::MonsterInit(FMonsterStatus status) {

	auto DataTable = LoadObject<UDataTable>(NULL, *(status.PartsDataRef), NULL, LOAD_None, NULL);
	MonsterName = status.MonsterName;
	HitBox->InitHitBox(DataTable,GetMesh());
	Status->SetStatus(status.HP, status.Damage);
	Status->DeadDel.AddUObject(this, &ABossDragon::Dead);
	AnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	GetDamageCapsules();

	MonsterController = Cast<AMonsterAIController>(GetController());

	return true;
}
void ABossDragon::Dead() {

	MonsterController->ChangeMonsterState(EMonsterState::E_DEAD);
}

void ABossDragon::ChangeMonsterState(EMonsterState state) {
	if (CurrentState == state) return;
	CurrentState = state;
	switch (CurrentState) {
		case EMonsterState::E_IDLE:
			GetCharacterMovement()->MaxWalkSpeed = BasicSpeed;
			break;
		case EMonsterState::E_BATTLE:
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
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

void ABossDragon::ChangeGroundToFly() {
	AnimInst->SetIsFly(true);
}
void ABossDragon::SetBrokenState(EMonsterPartsType brokenPart) {
	Cast<AMonsterAIController>(GetController())->SetBrokenState(brokenPart);
	Super::SetBrokenState(brokenPart);
}
void ABossDragon::PartBroken(EMonsterPartsType brokenPart) {
	switch (brokenPart)
	{
	case EMonsterPartsType::E_HEAD:
		Wounds[0]->SetVisibility(true);
		Wounds[1]->SetVisibility(true);
		break;
	case EMonsterPartsType::E_TAIL:
		Wounds[2]->SetVisibility(true);
		Wounds[3]->SetVisibility(true);
		Wounds[4]->SetVisibility(true);
		break;
	}
}


void ABossDragon::Roar() {

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

void ABossDragon::ShootFireBall() {
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
ADragonFireBall*  ABossDragon::FindFireBall() {
	if (FireBalls.Num() <= 0) return nullptr;

	for (auto fireball : FireBalls) {
		if (!fireball->GetIsWorking()) {
			return fireball;
		}
	}
	return nullptr;
}
