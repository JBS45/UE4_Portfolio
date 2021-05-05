// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "../Components/MyPlayerCameraManager.h"
#include "../Monster/BaseMonster.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Components/DamageInterface.h"
#include "../Input/InputBufferManager.h"
#include "../Input/CommandTableManager.h"


ABasePlayerController::ABasePlayerController() {
	PlayerCameraManagerClass = AMyPlayerCameraManager::StaticClass();

	InputBuffer = CreateDefaultSubobject<UInputBufferManager>(TEXT("InputBuffer"));
	CommandTable = CreateDefaultSubobject<UComandTableManager>(TEXT("CommandTable"));

	IsLockOn = false;

	CurrentWeapon = EWeaponType::E_NOWEAPON;
}

void ABasePlayerController::SetupInputComponent(){
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("UpDown"), this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &ABasePlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("LookUP"), this, &ABasePlayerController::CameraLookUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &ABasePlayerController::CameraTurn);

	InputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &ABasePlayerController::ToggleLockOn);
	InputComponent->BindAction(TEXT("LockChnage"), EInputEvent::IE_Pressed, this, &ABasePlayerController::ChangeLockOnTarget);


	InputComponent->BindAction(TEXT("LeftShift"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressShift);
	InputComponent->BindAction(TEXT("LeftShift"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseShift);

	InputComponent->BindAction(TEXT("Evade"), EInputEvent::IE_Pressed, this, &ABasePlayerController::SpaceKey);
	InputComponent->BindAction(TEXT("MouseLeftClick"), EInputEvent::IE_Pressed, this, &ABasePlayerController::MouseLeftClick);
	InputComponent->BindAction(TEXT("MouseRightClick"), EInputEvent::IE_Pressed, this, &ABasePlayerController::MouseRightClick);

	InputComponent->BindAction(TEXT("W"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressW);
	InputComponent->BindAction(TEXT("W"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseW);
	InputComponent->BindAction(TEXT("S"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressS);
	InputComponent->BindAction(TEXT("S"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseS);
	InputComponent->BindAction(TEXT("D"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressD);
	InputComponent->BindAction(TEXT("D"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseD);
	InputComponent->BindAction(TEXT("A"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressA);
	InputComponent->BindAction(TEXT("A"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseA);

}

void ABasePlayerController::BeginPlay() {
	Super::BeginPlay();

	TESTLOG(Warning, TEXT("player control begin play"));
}

void ABasePlayerController::OnPossess(APawn* pawn) {
	Super::OnPossess(pawn);

	TESTLOG(Warning, TEXT("player control possess"));
	
	CurrentCharaceter = Cast<ABaseCharacter>(this->GetPawn());
	CurrentCharaceter->SetInitWeapon();

	ChangeStateDel.AddUObject(this, &ABasePlayerController::ChangeCharacterState);
	ChangeStateDel.AddUObject(CurrentCharaceter, &ABaseCharacter::CharacterChangeState);
	ChangeStateDel.AddUObject(InputBuffer, &UInputBufferManager::ChangeCharacterState);

	ChangeWeaponDel.AddUObject(this, &ABasePlayerController::ChangeWeapon);
	ChangeWeaponDel.AddUObject(CurrentCharaceter, &ABaseCharacter::ChangeWeapon);
	ChangeWeaponDel.AddUObject(CommandTable, &UComandTableManager::ChangeCommandTable);
	ChangeWeaponDel.AddUObject(InputBuffer, &UInputBufferManager::ChangeWeapon);

	InputBuffer->SetCommandTable(CommandTable);

	CurrentCharaceter->GetCharacterStatus()->OnCharacterDeadDel.BindUObject(this, &ABasePlayerController::OnDead);
	CurrentCharaceter->GetCharacterStatus()->OnStaminaZeroDel.BindUObject(this, &ABasePlayerController::NoStatmina);

	SprintDel.AddUObject(CurrentCharaceter, &ABaseCharacter::SetIsSprint);
	SprintDel.AddUObject(CurrentCharaceter->GetCharacterStatus(), &UCharacterStatusManager::SetIsSprint);


	AttachWidgetToViewport(HUDWidgetClass);

	PlayerHUD->BindCharacterStatus(CurrentCharaceter->GetCharacterStatus());

	CommandTable->NewActionChainDel.BindUObject(PlayerHUD, &UBaseWidget::UpdateCommand);
	CurrentCharaceter->AttackDel.BindUObject(PlayerHUD, &UBaseWidget::UseDamageText);
}


void ABasePlayerController::MoveForward(float Value)
{
	if (!CanMove) return;

	CurrentCharaceter->MoveForward(Value);
	if (Value > 0.0f) {
		PressW();
	}
	else if (Value < 0.0f) {
		PressS();
	}
}
void ABasePlayerController::MoveRight(float Value)
{
	if (!CanMove) return;

	if (CurrentCharaceter)
	{
		CurrentCharaceter->MoveRight(Value);
		if (Value > 0.0f) {
			PressD();
		}
		else if (Value < 0.0f) {
			PressA();
		}
	}
}

void ABasePlayerController::PressW()
{
	FMoveInputInfo input;
	input.InputType = EMoveKey::E_FORWARD;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer -> AddInputToInputBuffer(input);
	InputBuffer->SetPressedW(true);
}
void ABasePlayerController::ReleaseW()
{
	InputBuffer->SetPressedW(false);
}
void ABasePlayerController::PressS()
{
	FMoveInputInfo input;
	input.InputType = EMoveKey::E_BACKWARD;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
	InputBuffer->SetPressedS(true);
}
void ABasePlayerController::ReleaseS() 
{
	InputBuffer->SetPressedS(false);
}
void ABasePlayerController::PressD()
{
	FMoveInputInfo input;
	input.InputType = EMoveKey::E_RIGHT;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
	InputBuffer->SetPressedD(true);
}
void ABasePlayerController::ReleaseD()
{
	InputBuffer->SetPressedD(false);
}
void ABasePlayerController::PressA()
{
	FMoveInputInfo input;
	input.InputType = EMoveKey::E_LEFT;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
	InputBuffer->SetPressedA(true);
}
void ABasePlayerController::ReleaseA()
{
	InputBuffer->SetPressedA(false);
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
/*void ABasePlayerController::CameraZoom(float Value) {

	if (CurrentCharaceter)
	{
		CurrentCharaceter->CameraZoom(Value);
	}
}*/
void ABasePlayerController::PressShift() {

	IsSprint = true;
	SprintDel.Broadcast(IsSprint);

	InputBuffer->PutUpWeapon();
}
void ABasePlayerController::ReleaseShift() {

	IsSprint = false;
	SprintDel.Broadcast(IsSprint);
}

void ABasePlayerController::MouseLeftClick() {
	//AddInputBuffer(EInputKey::E_LEFTCLICK);
	FActionInputInfo input;
	input.InputType = EActionKey::E_LEFTCLICK;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
}
void ABasePlayerController::MouseRightClick() {
	//AddInputBuffer(EInputKey::E_RIGHTCLICK);
	FActionInputInfo input;
	input.InputType = EActionKey::E_RIGHTCLICK;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
}
void ABasePlayerController::SpaceKey() {
	//AddInputBuffer(EInputKey::E_EVADE);
	FActionInputInfo input;
	input.InputType = EActionKey::E_EVADE;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
}

void ABasePlayerController::ChangeCharacterState(ECharacterState state) {
	if (CharacterState == state) return;
	CharacterState = state;
	switch (CharacterState) {
	case ECharacterState::E_IDLE:
		CanMove = true;
		break;
	case ECharacterState::E_BATTLE:
		CanMove = true;
		break;
	case ECharacterState::E_HIT:
		CanMove = false;
		break;
	case ECharacterState::E_DOWN:
		CanMove = false;
		break;
	case ECharacterState::E_DEAD:
		CanMove = false;
		break;
	}
}
void ABasePlayerController::ChangeWeapon(EWeaponType type) {
	if (CurrentWeapon == type)return;
	CurrentWeapon = type;
	
}
void ABasePlayerController::ToggleLockOn()
{
	if (IsLockOn) {
		IsLockOn = false;
		PlayerHUD->SetCameraLockOn(false);
	}
	else {
		auto Array = CurrentCharaceter->GetTargetMonster();
		if (Array.Num() > 0) {
			if (Target == nullptr) {
				Target = Cast<ABaseMonster>(Array[0]);
			}
			else {
				if (!Array.Contains(Target)) {
					Target = Cast<ABaseMonster>(Array[0]);
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
			}
		}
	}
	//Todo : Next Target from iterator;
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

void ABasePlayerController::OnDead() {
	ChangeStateDel.Broadcast(ECharacterState::E_DEAD);
}
void ABasePlayerController::NoStatmina() {
	IsSprint = false;
	SprintDel.Broadcast(false);
}
UInputBufferManager* ABasePlayerController::GetInputBuffer() {
	return InputBuffer;
}