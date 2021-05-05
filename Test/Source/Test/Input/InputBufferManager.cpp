// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferManager.h"
#include "CommandTableManager.h"
#include "../Player/BaseCharacter.h"
#include "../Player/BasePlayerController.h"

// Sets default values for this component's properties
UInputBufferManager::UInputBufferManager() 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CurrentWeapon = EWeaponType::E_NOWEAPON;
	CurrentActionName = "Base";
}


// Called when the game starts
void UInputBufferManager::BeginPlay()
{
	Super::BeginPlay();


	//CommandTable->ActiveActionDel.AddUObject(this, &UInputBufferManager::SetCurrentAction);
	// ...
	ChainReset();
}


// Called every frame
void UInputBufferManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ActionBufferCheck();
	MoveBufferCheck();
	//CheckBufferTime();
	// ...
}

void UInputBufferManager::AddInputToInputBuffer(FMoveInputInfo input)
{
	if (!CanBufferInput) return;

	switch (input.InputType) {
		case EMoveKey::E_FORWARD:
			MoveAddInput(bPressedW,input);
			break;
		case EMoveKey::E_BACKWARD:
			MoveAddInput(bPressedS, input);
			break;
		case EMoveKey::E_RIGHT:
			MoveAddInput(bPressedD, input);
			break;
		case EMoveKey::E_LEFT:
			MoveAddInput(bPressedA, input);
			break;
	}

}
void UInputBufferManager::MoveAddInput(bool Pressed, FMoveInputInfo input)
{
	if (MoveInputBuffer.Num() <= 0) return;

	if (Pressed) {
		int index = MoveInputBuffer.FindLastByPredicate([input](const FMoveInputInfo Lhs) {return Lhs.InputType == input.InputType;});
		MoveInputBuffer[index].TimeStamp = input.TimeStamp;
	}
	else {
		if (MoveInputBuffer.Num() > 5) {
			MoveInputBuffer.RemoveAt(0);
		}
		MoveInputBuffer.Add(input);;
	}
}

void UInputBufferManager::AddInputToInputBuffer(FActionInputInfo input)
{
	if (!CanBufferInput) return;

	if (ActionInputBuffer.Num() > 2) {
		ActionInputBuffer.RemoveAt(0);
	}
	ActionInputBuffer.Add(input);;
}

void UInputBufferManager::ChainReset() {
	switch (CurrentState) {
	case ECharacterState::E_BATTLE:
		ReadyForNextAction(COMMAND_DRAWWEAPON);
		break;
	default:
		ReadyForNextAction(COMMAND_BASE_STATE);
		break;
	}
	CanBufferInput = true;
}

void UInputBufferManager::PutUpWeapon() {
	if (CurrentState == ECharacterState::E_BATTLE) {
		PlayAnimation(COMMAND_PUTUPWEAPON);
	}
}


void UInputBufferManager::ActionBufferCheck()
{
	
	if (ActionInputBuffer.Num() <= 0) return;

	int last = ActionInputBuffer.Num() - 1;
	if (ActionInputBuffer[last].InputType == EActionKey::E_EVADE) {
		PlayAnimation(COMMAND_EVADE);
		ActionInputBuffer.Empty();
		MoveInputBuffer.Empty();
	}
	else if (ActionInputBuffer[last].TimeStamp + INPUT_BUFFER_DELAY <= GetWorld()->GetTimeSeconds()) {
		CheckCommand();
	}
	for (int i = 0; i < ActionInputBuffer.Num();) {
		if (ActionInputBuffer[i].TimeStamp + (2 * INPUT_BUFFER_DELAY) <= GetWorld()->GetTimeSeconds()) {
			ActionInputBuffer.RemoveAt(i);
			continue;
		}
		i++;
	}
}
void UInputBufferManager::MoveBufferCheck() {
	for (int i = 0; i < MoveInputBuffer.Num();) {
		if (MoveInputBuffer[i].TimeStamp + INPUT_BUFFER_DELAY <= GetWorld()->GetTimeSeconds()) {
			MoveInputBuffer.RemoveAt(i);
			continue;
		}
		i++;
	}
}
bool UInputBufferManager::CheckCommand() {
	auto EnableActionName = CommandTable->FindEnableAction(CurrentActionName);

	TMap<ECommnadPriority, FString> CandidateAction;
	
	for (auto ActionName : EnableActionName)
	{
		FChainAction AciontInfo = CommandTable->FindAction(ActionName);
		TArray<EActionKey> ActionCommands = AciontInfo.ActionCommand;
		TArray<EMoveKey> MoveCommands = AciontInfo.MoveCommand;

		float MoveTimer = 0.0f;
		float ActionTimer = 0.0f;

		bool ValidMove = true;
		if (MoveCommands.Num() > 0) {
			ValidMove = MoveCommnadCheck(MoveCommands, MoveTimer);
		}
		bool ValidAction = ActionCommnadCheck(ActionCommands, ActionTimer);

		if (ValidMove && ValidAction && (MoveTimer <= ActionTimer)) {
			CandidateAction.Add(AciontInfo.Priority, AciontInfo.AttackName);
		}
	}
	if (CandidateAction.Contains(ECommnadPriority::E_HIGH)) {
		auto actionName = CandidateAction.Find(ECommnadPriority::E_HIGH);
		PlayAnimation(*actionName);
		//Active this;
	}
	else if (CandidateAction.Contains(ECommnadPriority::E_NORMAL)) {
		auto actionName = CandidateAction.Find(ECommnadPriority::E_NORMAL);
		PlayAnimation(*actionName);
	}
	else if(CandidateAction.Contains(ECommnadPriority::E_LOW)) {
		auto actionName = CandidateAction.Find(ECommnadPriority::E_LOW);
		PlayAnimation(*actionName);
	}
	else {
		return false;
	}
	ActionInputBuffer.Empty();
	MoveInputBuffer.Empty();

	return true;
}

bool UInputBufferManager::MoveCommnadCheck(TArray<EMoveKey> Command, float& LastInput)
{

	LastInput = MAX_flt;
	if (MoveInputBuffer.Num() <= 0) return false;

	if (Command.Num() == 0) {
		return true;
	}

	if (Command.Num() == 1) {
		
		int last = MoveInputBuffer.Num() - 1;

		if ((Command[0] == EMoveKey::E_ALLMOVE && MoveInputBuffer.Num() > 0) 
			|| (Command[0] == MoveInputBuffer[last].InputType)) {
			LastInput = MoveInputBuffer[last].TimeStamp;
			return true;
		}
	}

	if (Command.Num() == 2) {
		int last = MoveInputBuffer.Num() - 1;

		if (Command[0] == MoveInputBuffer[last - 1].InputType
			&& Command[1] == MoveInputBuffer[last].InputType
			&& (MoveInputBuffer[last].TimeStamp - MoveInputBuffer[last - 1].TimeStamp)<= INPUT_BUFFER_DELAY) {
			LastInput = MoveInputBuffer[last].TimeStamp;
			return true;
		}
	}
	return false;

}
bool UInputBufferManager::ActionCommnadCheck(TArray<EActionKey> Command, float& FirstInput)
{
	FirstInput = MIN_flt;

	if (Command.Num() == 1 && ActionInputBuffer.Num() >= 1) {
		int last = ActionInputBuffer.Num() - 1;

		if ((Command[0] == ActionInputBuffer[0].InputType)){
			FirstInput = ActionInputBuffer[last].TimeStamp;
			return true;
		}
	}

	if (Command.Num() == 2 && ActionInputBuffer.Num()>=2) {
		int last = ActionInputBuffer.Num() - 1;

		if ((Command[0] == ActionInputBuffer[last - 1].InputType)
			&& (Command[1] == ActionInputBuffer[last].InputType)
			||((Command[1] == ActionInputBuffer[last - 1].InputType)
				&& (Command[0] == ActionInputBuffer[last].InputType)))
		{
			if ((ActionInputBuffer[last].TimeStamp - ActionInputBuffer[last - 1].TimeStamp) <= INPUT_BUFFER_DELAY) {
				FirstInput = ActionInputBuffer[last].TimeStamp;
				return true;
			}
		}
	}

	return false;
}


void UInputBufferManager::ReadyForNextAction(FString newActionName) {
	CurrentActionName = newActionName;
	CommandTable->SetCurrentCommandName(newActionName);
}
void UInputBufferManager::ChangeCharacterState(ECharacterState state) {
	if (CurrentState == state) return;
	CurrentState = state;
	switch (CurrentState) {
	case ECharacterState::E_IDLE:
		CanBufferInput = true;
		break;
	case ECharacterState::E_BATTLE:
		CanBufferInput = true;
		break;
	case ECharacterState::E_HIT:
		CanBufferInput = false;
		break;
	case ECharacterState::E_DOWN:
		CanBufferInput = false;
		break;
	case ECharacterState::E_DEAD:
		CanBufferInput = false;
		break;
	}
}
void  UInputBufferManager::ChangeWeapon(EWeaponType weapon) {
	if (CurrentWeapon == weapon) return;
	CurrentWeapon = weapon;

	switch (CurrentWeapon) {
	case EWeaponType::E_NOWEAPON:
		break;
	case EWeaponType::E_DUAL:
		break;
	}

	CurrentActionName = "Base";
	ChainReset();
}

void UInputBufferManager::SetCommandTable(UComandTableManager* table) {
	CommandTable = table;
}

void UInputBufferManager::InputBufferOpen() 
{ 
	CanBufferInput = true;
}
void UInputBufferManager::InputBufferClose() 
{ 
	CanBufferInput = false;
}
void UInputBufferManager::SetCurrentAction(FString actionName) {
	CurrentActionName = actionName;
}
void UInputBufferManager::SetPressedW(bool value) {
	bPressedW = value;
}
void UInputBufferManager::SetPressedS(bool value) {
	bPressedS = value;
}
void UInputBufferManager::SetPressedD(bool value) {
	bPressedD = value;
}
void UInputBufferManager::SetPressedA(bool value) {
	bPressedA = value;
}

void UInputBufferManager::PlayAnimation(FString actionName) {
	auto Anim = CommandTable->FindAction(*actionName);
	ReadyForNextAction(*actionName);
	PlayAnimEventDel.Execute(Anim.ActionMontage);
}
/*bool FMoveInputInfo::operator==(const FMoveInputInfo& other) const {
	if (this->InputType == other.InputType) {
		if ((other.TimeStamp - this->TimeStamp) <= INPUT_BUFFER_DELAY*2) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}*/
