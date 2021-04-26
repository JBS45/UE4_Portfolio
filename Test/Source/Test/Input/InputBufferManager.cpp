// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferManager.h"
#include "../LoadAssetStruct.h"
#include "../Player/BaseCharacter.h"
#include "../Player/BasePlayerController.h"

// Sets default values for this component's properties
UInputBufferManager::UInputBufferManager() 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	removeInputFromBufferTime = 0.15f;
	CurrentWeapon = EWeaponType::E_NOWEAPON;

	UDataTable* TmpTable = nullptr;
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/DataTable/Command/CommandTable.CommandTable'"));
	if (DataTable.Succeeded())
	{
		TmpTable = DataTable.Object;
	}

	if (IsValid(TmpTable)) {
	TArray<FName> RowNames = TmpTable->GetRowNames();

		for (auto name : RowNames)
		{
			static const FString ContextString(TEXT("Set CommandTable"));
			auto CommandTableRef = TmpTable->FindRow<FLoadAssetTable>(name, ContextString, true)->reference;
			auto type = TmpTable->FindRow<FLoadAssetTable>(name, ContextString, true)->weapon;
			ConstructorHelpers::FObjectFinder<UDataTable> TEMP(*CommandTableRef);

			UDataTable* WeaponCommand = nullptr;
			if (TEMP.Succeeded()) {
				WeaponCommand = TEMP.Object;
			}
			auto Names = WeaponCommand->GetRowNames();
			TMap<FString, FChainAction> Commands;
			for (auto rowName : Names) {
				auto TempActionChain = WeaponCommand->FindRow<FChainActionTable>(rowName, ContextString, true)->Action;
				Commands.Add(TempActionChain.AttackName, TempActionChain);
			}
			TotalCommands.Add(type, Commands);
		}
	}

	CanAcitonInput = true;
}


// Called when the game starts
void UInputBufferManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ChainReset();
}


// Called every frame
void UInputBufferManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (BufferCheckDelay()) {
		CheckInputBufferCommand();
		CheckBufferTime();
	}
	//CheckBufferTime();
	// ...
}

void UInputBufferManager::AddInputToInputBuffer(FInputInfo input)
{
	if (!CanAcitonInput) return;
	if (InputBuffer.Contains(input)) return;

	if (InputBuffer.Num() > 5) {
		InputBuffer.RemoveAt(0);
	}
	InputBuffer.Add(input);;
}

void  UInputBufferManager::CheckInputBufferCommand()
{
	int correctSequenceCounter = 0;
	TArray<FChainAction*> ValidAction;

	for (auto action : *EnableAction) {
		auto TempAction = CurrentCommands->Find(action);
		auto TempCommands = TempAction->Commands;
		for (auto variation : TempCommands) {
			for (int CommandInput = 0; CommandInput < variation.Inputs.Num(); ++CommandInput) {
				for (int input = correctSequenceCounter; input < InputBuffer.Num(); ++input) {
					if (InputBuffer[input].InputType == variation.Inputs[CommandInput]) {
						++correctSequenceCounter;
						if (correctSequenceCounter == variation.Inputs.Num()) {
							ValidAction.Add(TempAction);
						}
						break;
					}
					else {
						correctSequenceCounter = 0;
					}

				}
			}

		}
	}
	if (ValidAction.Num() > 0) {
		ValidAction.Sort([](const FChainAction& LHS, const FChainAction& RHS) { return LHS.Priority > RHS.Priority; });
		if ((ValidAction[0])->AttackName == COMMAND_EVADE){
			EvadeDelegate.ExecuteIfBound();
			InputBuffer.Empty();
		}
		else {
			TargetAnimInst->PlayAnimMontage(ActiveCommand((ValidAction[0])->AttackName));
		}
	}
}
void UInputBufferManager::CheckBufferTime() {
	for (int i = 0; i < InputBuffer.Num();) {
		if (InputBuffer[i].TimeStamp + removeInputFromBufferTime <= GetWorld()->GetTimeSeconds()) {
			InputBuffer.RemoveAt(i);
		}
		else {
			++i;
		}
	}
}
void UInputBufferManager::ChangeCommandDT(EWeaponType weapon) {
	if (CurrentWeapon == weapon) return;
	CurrentWeapon = weapon;

	CurrentCommands = nullptr;

	switch (CurrentWeapon) {
		case EWeaponType::E_NOWEAPON:
			CurrentCommands = &TotalCommands[EWeaponType::E_NOWEAPON];
			break;
		case EWeaponType::E_DUAL:
			CurrentCommands = &TotalCommands[EWeaponType::E_DUAL];
			break;
	}
	ActiveCommand(COMMAND_BASE_STATE);
	//TESTLOG(Warning, TEXT("%d"), CurrentCommands->Num());
}
UAnimMontage* UInputBufferManager::ActiveCommand(FString NextCommand) {
	if (CurrentCommands->Num() <= 0) {
		CurrentCommandName = NextCommand;
		return nullptr;
	}
	auto TempAnimMontage = CurrentCommands->Find(NextCommand)->ActionMontage;
	EnableAction = &(CurrentCommands->Find(NextCommand)->EnableChainAction);

	CommandUpdateDelegate.ExecuteIfBound();

	CurrentCommandName = NextCommand;
	InputBuffer.Empty();

	return TempAnimMontage;
}


bool UInputBufferManager::BufferCheckDelay() {
	if (InputBuffer.Num() > 0) {
		auto Last = InputBuffer.Last();

		float TimeGap = GetWorld()->GetTimeSeconds() - Last.TimeStamp;
		if (TimeGap <= INPUT_BUFFER_DELAY) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return true;
	}
}
void UInputBufferManager::ChainReset() {
	switch (CurrentState) {
	case ECharacterState::E_BATTLE:
		ActiveCommand(COMMAND_DRAWWEAPON);
		break;
	case ECharacterState::E_IDLE:
	case ECharacterState::E_SPRINT:
	case ECharacterState::E_EXHAUST:
		ActiveCommand(COMMAND_BASE_STATE);
		break;
	}
	CanAcitonInput = true;
}
void UInputBufferManager::PutUpWeapon() {
	if (CurrentState == ECharacterState::E_BATTLE) {
		TargetAnimInst->PlayAnimMontage(ActiveCommand(COMMAND_PUTUPWEAPON));
		CanAcitonInput = true;
	}
}

bool FInputInfo::operator==(const FInputInfo& other) const {
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
}