// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "../BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "BasePlayerController.h"
#include "../Monster/BaseMonster.h"
#include "../Components/DetectComponent.h"

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

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_MESH(TEXT("StaticMesh'/Game/Meshes/Talon.Talon'"));
	if (SM_MESH.Succeeded()) {
		TempMesh = SM_MESH.Object;
	}

	IsDrawWeapon = false;


	_MovementStatus = CreateDefaultSubobject<UMovementManager>(TEXT("MOVEMENT"));
	_MovementStatus->SetCharacterMovement(GetCharacterMovement());
	_MovementStatus->SetIsSprintForStat.BindUObject(this, &ABaseCharacter::SetIsSprint);

	_StatusManager = CreateDefaultSubobject<UCharacterStatusManager>(TEXT("STATUSMANAGER"));
	_StatusManager->StatusInit();

	
	_CollisionManager = CreateDefaultSubobject<UHumanCollisionManager>(TEXT("COLLISIONMANAGER"));
	_CollisionManager->SetUpAttachSocket(GetMesh());
	_CollisionManager->SetUpCollisionType(FName(TEXT("PlayerHitBox")), ECollisionChannel::ECC_GameTraceChannel5);

	_Detect = CreateDefaultSubobject<UDetectComponent>(TEXT("DETECTMANAGER"));
	_Detect->SetDetectRange(DetectRange, EDetectCollisionType::E_MONSTER);
	_Detect->GetDetect()-> SetupAttachment(GetCapsuleComponent());

	_WeaponType = EWeaponType::E_NOWEAPON;



}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//TESTLOG_S(Warning);
	//TESTLOG(Warning, TEXT("Actor Name : %s, Location X : %.3f"), *GetName(), GetActorLocation().X);

	_AnimInst = Cast<UBaseCharAnimInstance>(GetMesh()->GetAnimInstance());

	if (_AnimInst != nullptr) {
		//TESTLOG(Warning, TEXT("valid"));
		_AnimInst->ExchangeWeapon(_WeaponType);
	}

	auto playerController = Cast<ABasePlayerController>(Controller);
	playerController->GetInputBuffer()->SetTargetAnimInst(_AnimInst);
	_StatusManager->OnChangeCharacterState.BindUObject(playerController, &ABasePlayerController::ChangeCharacterState);


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
	_Detect->DrawDebug(this, DeltaTime);
	auto control = Cast<ABasePlayerController>(GetController());
	control->CameraLockOn(DeltaTime);
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
	if (_StatusManager->UseStamina()) {
		_AnimInst->PlayEvade();
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
	if (_WeaponType == type) return;
	_WeaponType = type;

	switch (_WeaponType) {
		case EWeaponType::E_NOWEAPON:
			LeftHand->SetEnableWeapon(false);
			RightHand->SetEnableWeapon(false);
			break;
		case EWeaponType::E_DUAL:
			LeftHand->SetUpWeapon(EWeaponType::E_DUAL, TempMesh);
			RightHand->SetUpWeapon(EWeaponType::E_DUAL, TempMesh);
			LeftHand->SetEnableWeapon(true);
			RightHand->SetEnableWeapon(true);

			break;
		case EWeaponType::E_HAMMER:
			break;
	}

}
void ABaseCharacter::SetMovement(EMovementState state) {
	_MovementStatus->SetMovementState(state);
}
void ABaseCharacter::SetIsSprint(bool value) {
	_StatusManager->SetIsSprint(value);
}
TArray<APawn*> ABaseCharacter::GetTargetMonster() { 
	return _Detect->GetTargets(); 
}

void ABaseCharacter::SetInitWeapon() {
	LeftHand = GetWorld()->SpawnActor<ABaseWeapon>();
	RightHand = GetWorld()->SpawnActor<ABaseWeapon>();

	LeftHand->SetEnableWeapon(true);
	RightHand->SetEnableWeapon(true);

	LeftHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName(TEXT("LeftWeapon")));
	RightHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("RightWeapon")));
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