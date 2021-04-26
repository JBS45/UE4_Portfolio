// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "../Components/MyPlayerCameraManager.h"
#include "../Monster/BaseMonster.h"
#include "Kismet/KismetMathLibrary.h"


ABasePlayerController::ABasePlayerController() {
	PlayerCameraManagerClass = AMyPlayerCameraManager::StaticClass();

	InputBuffer = CreateDefaultSubobject<UInputBufferManager>(TEXT("InputBuffer"));
	IsLockOn = false;

	CurrentWeapon = EWeaponType::E_NOWEAPON;
}

void ABasePlayerController::SetupInputComponent(){
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("UpDown"), this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &ABasePlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("LookUP"), this, &ABasePlayerController::CameraLookUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &ABasePlayerController::CameraTurn);
	InputComponent->BindAxis(TEXT("Wheel"), this, &ABasePlayerController::CameraZoom);

	InputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &ABasePlayerController::ToggleLockOn);
	InputComponent->BindAction(TEXT("LockChnage"), EInputEvent::IE_Pressed, this, &ABasePlayerController::ChangeLockOnTarget);


	InputComponent->BindAction(TEXT("LeftShift"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressShift);
	InputComponent->BindAction(TEXT("LeftShift"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseShift);

	InputComponent->BindAction(TEXT("Evade"), EInputEvent::IE_Pressed, this, &ABasePlayerController::SpaceKey);
	InputComponent->BindAction(TEXT("MouseLeftClick"), EInputEvent::IE_Pressed, this, &ABasePlayerController::MouseLeftClick);
	InputComponent->BindAction(TEXT("MouseRightClick"), EInputEvent::IE_Pressed, this, &ABasePlayerController::MouseRightClick);


	InputComponent->BindAction(TEXT("TEST"), EInputEvent::IE_Pressed, this, &ABasePlayerController::TestKey);
}

void ABasePlayerController::BeginPlay() {
	Super::BeginPlay();

	TESTLOG(Warning, TEXT("player control begin play"));

	if (AssistantType == nullptr) {
		AssistantType = ABaseAssistant::StaticClass();
	}

	FVector location = CurrentCharaceter->GetActorLocation() - (CurrentCharaceter->GetActorForwardVector() * 100);
	FRotator rotator = FRotator::ZeroRotator;
	Assistant = GetWorld()->SpawnActor<ABaseAssistant>(AssistantType, location, rotator);
	Assistant->SetMaster(CurrentCharaceter);
}

void ABasePlayerController::OnPossess(APawn* pawn) {
	Super::OnPossess(pawn);

	TESTLOG(Warning, TEXT("player control possess"));
	
	CurrentCharaceter = Cast<ABaseCharacter>(this->GetPawn());
	CurrentCharaceter->SetInitWeapon();

	InputBuffer->EvadeDelegate.BindUObject(CurrentCharaceter, &ABaseCharacter::Evade);
	ChangeWeapon(EWeaponType::E_DUAL);
	ChangeCharacterState(ECharacterState::E_IDLE);

	AttachWidgetToViewport(HUDWidgetClass);
	PlayerHUD->BindCharacterStatus(CurrentCharaceter->GetCharacterStatus());
	PlayerHUD->BindInputCommand(InputBuffer);

}


void ABasePlayerController::MoveForward(float Value)
{

	if (CurrentCharaceter)
	{
		CurrentCharaceter->MoveForward(Value);
		if (Value > 0.0f) {
			AddInputBuffer(EInputKey::E_FORWARD);
		}
		else if (Value < 0.0f) {
			AddInputBuffer(EInputKey::E_BACKWARD);
		}
	}
}
void ABasePlayerController::MoveRight(float Value)
{

	if (CurrentCharaceter)
	{
		CurrentCharaceter->MoveRight(Value);
		if (Value > 0.0f) {
			AddInputBuffer(EInputKey::E_RIGHT);
		}
		else if (Value < 0.0f) {
			AddInputBuffer(EInputKey::E_LEFT);
		}
	}
}
void ABasePlayerController::CameraLookUp(float Value) {

	if (CurrentCharaceter)
	{
		CurrentCharaceter->CameraLockUp(Value);
	}
}
void ABasePlayerController::CameraTurn(float Value) {

	if (CurrentCharaceter)
	{
		CurrentCharaceter->CameraTurn(Value);
	}
}
void ABasePlayerController::CameraZoom(float Value) {

	if (CurrentCharaceter)
	{
		CurrentCharaceter->CameraZoom(Value);
	}
}
void ABasePlayerController::PressShift() {

	switch (CharacterState) {
	case ECharacterState::E_IDLE:
	case ECharacterState::E_BATTLE:
		InputBuffer->PutUpWeapon();
		ChangeCharacterState(ECharacterState::E_SPRINT);
		break;
	case ECharacterState::E_EXHAUST:
		break;
	}
}
void ABasePlayerController::ReleaseShift() {

	switch (CharacterState) {
	case ECharacterState::E_SPRINT:
	case ECharacterState::E_BATTLE:
		ChangeCharacterState(ECharacterState::E_IDLE);
		break;
	}
}

void ABasePlayerController::MouseLeftClick() {
	AddInputBuffer(EInputKey::E_LEFTCLICK);
}
void ABasePlayerController::MouseRightClick() {
	AddInputBuffer(EInputKey::E_RIGHTCLICK);
}
void ABasePlayerController::SpaceKey() {
	AddInputBuffer(EInputKey::E_EVADE);
}

void ABasePlayerController::TestKey() {

}

void ABasePlayerController::ToggleLockOn()
{
	if (IsLockOn) {
		IsLockOn = false;
		PlayerHUD->SetCameraLockOn(false);
	}
	else {
		auto Array = CurrentCharaceter->GetTargetMonster();
		TESTLOG(Warning, TEXT("%d"), Array.Num());
		if (Array.Num() > 0) {
			if (Target == nullptr) {
				Target = Cast<ABaseMonster>(Array[0]);
				Assistant->SetTarget(Target);
			}
			else {
				if (!Array.Contains(Target)) {
					Target = Cast<ABaseMonster>(Array[0]);
					Assistant->SetTarget(Target);
				}
			}
		}

		if (Target != nullptr) {
			IsLockOn = true;
			PlayerHUD->SetCameraLockOn(true);
		}

	}
}

void ABasePlayerController::ChangeLockOnTarget()
{
	if (IsLockOn) {
		auto Array = CurrentCharaceter->GetTargetMonster();
		if (Array.Num() > 0) {
			if (Target != nullptr) {
				auto iter = Array.Find(Target);
				iter++;
				if (iter > Array.Num() - 1) {
					iter = 0;
				}
				Target = Cast<ABaseMonster>(Array[iter]);
				Assistant->SetTarget(Target);
			}
		}
	}
	//Todo : Next Target from iterator;
}

void ABasePlayerController::AddInputBuffer(EInputKey key)
{
	FInputInfo TempInputInfo;

	TempInputInfo.InputType = key;
	TempInputInfo.TimeStamp = GetWorld()->GetTimeSeconds();
	InputBuffer->AddInputToInputBuffer(TempInputInfo);


}

void ABasePlayerController::ChangeCharacterState(ECharacterState state) {
	if (CharacterState == state) return;
	CharacterState = state;
	switch (CharacterState) {
	case ECharacterState::E_IDLE:
		InputBuffer->SetCharacterState(CharacterState);
		CurrentCharaceter->SetMovement(EMovementState::E_IDLE);
		CurrentCharaceter->SetIsDrawWeapon(false);
		break;
	case ECharacterState::E_EXHAUST:
		InputBuffer->SetCharacterState(CharacterState);
		CurrentCharaceter->SetMovement(EMovementState::E_EXHAUST);
		CurrentCharaceter->SetIsDrawWeapon(false);
		CurrentCharaceter->GetAnimInst()->PlayExhaust();
		break;
	case ECharacterState::E_SPRINT:
		InputBuffer->SetCharacterState(CharacterState);
		CurrentCharaceter->SetMovement(EMovementState::E_SPRINT);
		CurrentCharaceter->SetIsDrawWeapon(false);
		break;
	case ECharacterState::E_BATTLE:
		InputBuffer->SetCharacterState(CharacterState);
		CurrentCharaceter->SetMovement(EMovementState::E_IDLE);
		CurrentCharaceter->SetIsDrawWeapon(true);
		Assistant->SetIsBattle(true);
		break;
	case ECharacterState::E_ATTACKING:
		break;
	case ECharacterState::E_EVADE:
		break;
	case ECharacterState::E_HIT:
		break;
	case ECharacterState::E_KNOCKBACK:
		InputBuffer->SetCharacterState(CharacterState);
		CurrentCharaceter->SetIsDrawWeapon(false);
		break;
	case ECharacterState::E_DOWN:
		InputBuffer->SetCharacterState(CharacterState);
		CurrentCharaceter->SetIsDrawWeapon(false);
		break;
	case ECharacterState::E_DEAD:
		InputBuffer->SetCharacterState(CharacterState);
		CurrentCharaceter->SetIsDrawWeapon(false);
		break;
	}
}
void ABasePlayerController::ChangeWeapon(EWeaponType type) {
	if (CurrentWeapon == type)return;
	CurrentWeapon = type;
	if (CurrentCharaceter)
	{
		CurrentCharaceter->ChangeWeapon(CurrentWeapon);
		InputBuffer->ChangeCommandDT(CurrentWeapon);
	}
}

void ABasePlayerController::AttachWidgetToViewport(TSubclassOf<class UBaseWidget> widget) {
	if (widget != nullptr) {
		PlayerHUD = CreateWidget<UBaseWidget>(this, widget);

		if (PlayerHUD != nullptr) {
			PlayerHUD->AddToViewport();
		}
	}
}
void ABasePlayerController::CameraLockOn(float deltaTime) {
	if (IsLockOn) {
		auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentCharaceter->GetActorLocation(), Target->GetActorLocation());
		auto TempRot = FMath::RInterpTo(GetControlRotation(), TargetRotation, deltaTime, 10.0f);
		SetControlRotation(TempRot);

		PlayerHUD->TraceTarget(this,Target);

		float Turn = FMath::Abs(InputComponent->GetAxisValue("Turn"));
		float LookUp = FMath::Abs(InputComponent->GetAxisValue("LookUP"));
		float Distance = FVector::Distance(CurrentCharaceter->GetActorLocation(), Target->GetActorLocation());
		if (Turn >= 2.0f || LookUp >= 2.0f|| Distance> CurrentCharaceter->GetDetectRange()) {
			IsLockOn = false;
			PlayerHUD->SetCameraLockOn(false);
		}
	}

}
ABaseMonster* ABasePlayerController::GetTarget() { 
	return Target;
}