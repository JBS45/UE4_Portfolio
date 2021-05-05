// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "../BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "BasePlayerController.h"
#include "../Monster/BaseMonster.h"
#include "../Monster/MonsterAIController.h"
#include "../Components/DetectComponent.h"
#include "Components/PostProcessComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "../Input/InputBufferManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Player")));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetCapsuleComponent()->bDynamicObstacle = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);


	/*Mesh*/
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	CurrentSpringArmLength = 400.0f;
	WheelSpeed = 25.0f;
	SpringArm->TargetArmLength = CurrentSpringArmLength;
	BaseCameraVector = FVector(0.0f, 0.0f, 20.0f);
	BaseCameraRotator = FRotator(-15.0f, 0.0f, 0.0f);
	SpringArm->SetRelativeLocation(BaseCameraVector);
	SpringArm->SetRelativeRotation(BaseCameraRotator);
	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->MaxAcceleration = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeed = BasicSpeed;

	/*Mesh */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MESH(TEXT("/Game/ODSMannequin/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	if (SK_MESH.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MESH.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	/*Mesh */
	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/Blueprints/Anim/Player/DualAnimaitonBP.DualAnimaitonBP_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>WEAPON_MESH(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Dual_Blade/Dual_Blade_Talon/SK_Dual_Blade_Talon.SK_Dual_Blade_Talon'"));
	if (WEAPON_MESH.Succeeded()) {
		TempMesh = WEAPON_MESH.Object;
	}
	
	CurrentState = ECharacterState::E_NONE;

	StatusManager = CreateDefaultSubobject<UCharacterStatusManager>(TEXT("STATUSMANAGER"));
	StatusManager->StatusInit();

	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel6);
	GetMesh()->SetCollisionProfileName("PlayerHitBox");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	
	CollisionManager = CreateDefaultSubobject<UHumanCollisionManager>(TEXT("COLLISIONMANAGER"));
	CollisionManager->SetUpCharacterStatusManager(this);
	CollisionManager->InitHitBox(nullptr,GetMesh());

	Detect = CreateDefaultSubobject<UDetectComponent>(TEXT("DETECTMANAGER"));
	Detect->SetDetectRange(DetectRange, EDetectCollisionType::E_MONSTER);
	Detect->GetDetect()-> SetupAttachment(GetCapsuleComponent());


	PostProcessMat = CreateDefaultSubobject<UPostProcessComponent>(TEXT("POSTPROCESS"));

	WeaponType = EWeaponType::E_NOWEAPON;

	Tags.Add(PlayerTag);

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//TESTLOG_S(Warning);
	//TESTLOG(Warning, TEXT("Actor Name : %s, Location X : %.3f"), *GetName(), GetActorLocation().X);

	AnimInst = Cast<UBaseCharAnimInstance>(GetMesh()->GetAnimInstance());

	auto playerController = Cast<ABasePlayerController>(Controller);

	playerController->ChangeStateDel.AddUObject(AnimInst, &UBaseCharAnimInstance::CharacterChangeState);
	playerController->ChangeWeaponDel.AddUObject(AnimInst, &UBaseCharAnimInstance::ChangeWeapon);
	playerController->GetInputBuffer()->PlayAnimEventDel.BindUObject(AnimInst, &UBaseCharAnimInstance::PlayAnimMontage);

	playerController->ChangeStateDel.Broadcast(ECharacterState::E_IDLE);
	playerController->ChangeWeaponDel.Broadcast(EWeaponType::E_DUAL);



	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnOverlapBegin);
	GetMesh()->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnOverlapEnd);

}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//TESTLOG_S(Warning);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//TESTLOG_S(Warning);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Detect->DrawDebug(this, DeltaTime);
	auto control = Cast<ABasePlayerController>(GetController());
	control->CameraLockOn(DeltaTime);

	if (IsBlock) {
		Blocking(DeltaTime);
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::MoveForward(float NewAxisValue)
{
		float Dist = Camera->GetForwardVector().Size();
		FVector DirXY = FVector(Camera->GetForwardVector().X, Camera->GetForwardVector().Y, 0.0f);
		DirXY.Normalize();
		DirXY *= Dist;
		AddMovementInput(DirXY, NewAxisValue);
}

void ABaseCharacter::MoveRight(float NewAxisValue)
{
		float Dist = Camera->GetRightVector().Size();
		FVector DirXY = FVector(Camera->GetRightVector().X, Camera->GetRightVector().Y, 0.0f);
		DirXY.Normalize();
		DirXY *= Dist;
		AddMovementInput(DirXY, NewAxisValue);
}

void ABaseCharacter::CameraLockUp(float NewAxisValue) {
	AddControllerPitchInput(NewAxisValue);
}
void ABaseCharacter::CameraTurn(float NewAxisValue) {
	AddControllerYawInput(NewAxisValue);
}
void ABaseCharacter::Evade()
{
	if (StatusManager->UseStamina()) {
		AnimInst->PlayEvade();
	}
}
void ABaseCharacter::CameraZoom(float NewAxisValue) {
	CurrentSpringArmLength += WheelSpeed * NewAxisValue;
	float Dest = FMath::Clamp(CurrentSpringArmLength, MinCameraDistance, MaxCameraDistance);
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, Dest, GetWorld()->DeltaTimeSeconds, 10);
}



void ABaseCharacter::MoveCameraMaxDist() {
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, MaxCameraDistance, GetWorld()->DeltaTimeSeconds, 10);
}


void ABaseCharacter::ChangeWeapon(EWeaponType type) {
	if (WeaponType == type) return;
	WeaponType = type;

	switch (WeaponType) {
		case EWeaponType::E_NOWEAPON:
			LeftHand->SetEnableWeapon(false);
			RightHand->SetEnableWeapon(false);
			break;
		case EWeaponType::E_DUAL:
			LeftHand->SetUpWeapon(EWeaponType::E_DUAL, TempMesh, this);
			RightHand->SetUpWeapon(EWeaponType::E_DUAL, TempMesh, this);
			LeftHand->SetEnableWeapon(true);
			RightHand->SetEnableWeapon(true);
			break;
		case EWeaponType::E_HAMMER:
			break;
	}

}
TArray<APawn*> ABaseCharacter::GetTargetMonster() { 
	return Detect->GetTargets(); 
}

void ABaseCharacter::SetInitWeapon() {
	LeftHand = GetWorld()->SpawnActor<ABaseWeapon>();
	RightHand = GetWorld()->SpawnActor<ABaseWeapon>();

	LeftHand->SetEnableWeapon(true);
	RightHand->SetEnableWeapon(true);

	LeftHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName(TEXT("LeftWeapon")));
	RightHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName(TEXT("RightWeapon")));

	WeaponLeftOverlapOff();
	WeaponRightOverlapOff();
}

void ABaseCharacter::DrawWeapon() {
	if (IsValid(LeftHand) && IsValid(RightHand)) {

		LeftHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("LeftHand")));
		RightHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("RightHand")));

		LeftHand->SetActorRelativeLocation(FVector::ZeroVector);
		RightHand->SetActorRelativeLocation(FVector::ZeroVector);
	}
}
void ABaseCharacter::PutUpWeapon() {
	if (IsValid(LeftHand) && IsValid(RightHand)) {

		LeftHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("LeftWeapon")));
		RightHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("RightWeapon")));

		LeftHand->SetActorRelativeLocation(FVector::ZeroVector);
		RightHand->SetActorRelativeLocation(FVector::ZeroVector);
	}
}

bool ABaseCharacter::CaculateCritical() {
	auto CriticalChance = FMath::RandRange(0, 100);
	if (StatusManager->GetCritical() >= CriticalChance) {
		return true;
	}
	else {
		return false;
	}
}
void ABaseCharacter::RadialBlurOn() {
	if (PostProcessMat->Settings.WeightedBlendables.Array.Num() <= 0) return;
	auto BlurMat = PostProcessMat->Settings.WeightedBlendables.Array[0];
	if (BlurMat.Object != nullptr) {
		PostProcessMat->BlendWeight = 1.0f;
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.SetTimer(CheckTimer,this, &ABaseCharacter::RadialBlurOff, 1.5f, false, 1.5f);
	}
}
void ABaseCharacter::RadialBlurOff(){
	if (PostProcessMat->Settings.WeightedBlendables.Array.Num() <= 0) return;
	auto BlurMat = PostProcessMat->Settings.WeightedBlendables.Array[0];
	if (BlurMat.Object != nullptr) {
		PostProcessMat->BlendWeight = 0.0f;

	}
}

void ABaseCharacter::ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType, const float ImpactForce) {
	auto TargetActor = hit.Actor;

	float CaculateDamage = 0;

	bool IsCritical = false;
	bool IsWeak = false;
	int32 FinalDamage = 0;

	if (CaculateCritical()) {
		CaculateDamage = StatusManager->GetDamage() * 1.25f;
		IsCritical = true;

	}
	else {
		CaculateDamage = StatusManager->GetDamage();
		IsCritical = false;
	}

	CaculateDamage *= AcitonDamageRate;

	TESTLOG(Warning, TEXT("%f"), CaculateDamage);
	Cast<IDamageInterface>(TargetActor)->TakeDamageFunc(IsWeak, FinalDamage,this, hit, CaculateDamage, DamageType, ImpactForce);
	TESTLOG(Warning, TEXT("%d"), FinalDamage);
	AttackDel.Execute(Cast<ABasePlayerController>(GetController()), hit.ImpactPoint, FinalDamage, IsWeak);

}
void ABaseCharacter::TakeDamageFunc(bool& OutIsWeak, int32& OutFinalDamage, AActor* Causer,const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType, const float ImpactForce) {
	TESTLOG(Warning, TEXT("CaculateDamage : %f"), CaculateDamage);
	OutFinalDamage = StatusManager->TakeDamage(CaculateDamage);

	FVector Direction = GetActorLocation() - Causer->GetActorLocation();
	Direction.Normalize();
	Direction *= ImpactForce*3;

	auto playerController = Cast<ABasePlayerController>(Controller);
	playerController->ChangeStateDel.Broadcast(ECharacterState::E_HIT);

	switch (DamageType) {
	case EDamageType::E_NORMAL:
		break;
	case EDamageType::E_KNOCKBACK:
		TESTLOG(Warning, TEXT("KnockBack"));
		AnimInst->PlayKnockBack();
		Direction += GetActorUpVector()*ImpactForce;
		LaunchCharacter(Direction,false,false);
		break;
	case EDamageType::E_KNOCKDOWN:
		AnimInst->PlayKnockBack();
		Direction += GetActorUpVector()*ImpactForce;
		LaunchCharacter(Direction, false, false);
		break;
	case EDamageType::E_ROAR:
		RadialBlurOn();
		break;
	}
}

UBaseCharAnimInstance* ABaseCharacter::GetAnimInst() { 
	return AnimInst; 
}
EWeaponType ABaseCharacter::GetWeaponState() { 
	return WeaponType;
}
UCharacterStatusManager* ABaseCharacter::GetCharacterStatus() { 
	return StatusManager; 
}
bool ABaseCharacter::GetIsAlive() const { 
	return IsAlive; 
}
class ABaseWeapon* ABaseCharacter::GetLeftHand() {
	return LeftHand;
}
class ABaseWeapon* ABaseCharacter::GetRightHand() {
	return RightHand;
}

void ABaseCharacter::WeaponLeftOverlapOn() {
	LeftHand->SetOverlapEvent(true);
}
void ABaseCharacter::WeaponLeftOverlapOff() {
	LeftHand->SetOverlapEvent(false);
}
void ABaseCharacter::WeaponRightOverlapOn() {
	RightHand->SetOverlapEvent(true);
}
void ABaseCharacter::WeaponRightOverlapOff() {
	RightHand->SetOverlapEvent(false);
}
void ABaseCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	BlockTarget = OtherActor;
}

void ABaseCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	BlockTarget = nullptr;
}

FVector ABaseCharacter::BlockCheck(AActor* Actor) {

	float Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	FRotator Rotation = GetCapsuleComponent()->GetComponentRotation();

	FHitResult DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);
	GetWorld()->SweepSingleByChannel(
		DetectResult,
		Location,
		Location,
		Rotation.Quaternion(),
		ECC_GameTraceChannel3,
		FCollisionShape::MakeCapsule(Radius, Height),
		params);

	DrawDebugCapsule(GetWorld(), Location, Height, Radius, Rotation.Quaternion(), FColor::Orange, false, 1.0f);

	FVector Direction = FVector::ZeroVector;
	float Rate = 1.0f;
	if (DetectResult.Actor == Actor) {
		Direction = DetectResult.ImpactNormal*(-1.0f);
		Direction.Normalize();
		Rate = FVector::Distance(GetActorLocation(), DetectResult.ImpactPoint);
	}

	return Direction*(Radius - Rate);
}

void ABaseCharacter::Blocking(float delta) {
	FVector MovementVector = BlockCheck(BlockTarget);
	FVector Target = GetActorLocation() + MovementVector*0.5f;
	SetActorLocation(Target);
}
void ABaseCharacter::CharacterChangeState(ECharacterState state){
	if (CurrentState == state) return;
	CurrentState = state;

	switch (CurrentState) {
		case ECharacterState::E_IDLE:
			GetCharacterMovement()->Activate();
			IsBlock = true;
			break;
		case ECharacterState::E_BATTLE:
			IsBlock = true;
			break;
		case ECharacterState::E_HIT:
			IsBlock = false;
			break;
		case ECharacterState::E_DOWN:
			IsBlock = false;
			break;
		case ECharacterState::E_DEAD:
			GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("DEAD")));
			GetCharacterMovement()->Deactivate();
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			IsBlock = false;
			break;
	}
}
void ABaseCharacter::SetIsSprint(bool value) {
	if (value) {
		GetCharacterMovement()->MaxWalkSpeed = BasicSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}