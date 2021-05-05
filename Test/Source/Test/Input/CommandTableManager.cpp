// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandTableManager.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UComandTableManager::UComandTableManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	CurrentWeapon = EWeaponType::E_NOWEAPON;

	LoadCommandTable(TEXT("DataTable'/Game/DataTable/Command/TotalCommandTable.TotalCommandTable'"));
	CurrentCommandName = "Base";
}


// Called when the game starts
void UComandTableManager::BeginPlay()
{
	Super::BeginPlay();
}
TArray<FString> UComandTableManager::FindEnableAction(FString currentActionName) 
{
	auto EnableActionNames = CurrentCommands->Find(currentActionName)->EnableChainAction;

	//Connect HUD
	return EnableActionNames;
}

UAnimMontage* UComandTableManager::FindAnmation(FString actionName) {
	return CurrentCommands->Find(actionName)->ActionMontage;
}

void UComandTableManager::LoadCommandTable(const FString ref)
{
	UDataTable* TmpTable = nullptr;
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*ref);
	if (DataTable.Succeeded())
	{
		TmpTable = DataTable.Object;
	}

	if (IsValid(TmpTable)) {
		TArray<FName> RowNames = TmpTable->GetRowNames();

		for (auto name : RowNames)
		{
			static const FString ContextString(TEXT("Set CommandTable"));

			auto CommandTable = TmpTable->FindRow<FTotalCommandDataTable>(name, ContextString, true)->CommandTable;
			auto type = TmpTable->FindRow<FTotalCommandDataTable>(name, ContextString, true)->weapon;
			
			TMap<FString, FChainAction> Commands;

			for (auto rowName : CommandTable->GetRowNames()) 
			{
				auto TempActionChain = CommandTable->FindRow<FChainActionTable>(rowName, ContextString, true)->Action;

				Commands.Add(TempActionChain.AttackName, TempActionChain);
			}
			TotalCommands.Add(type, Commands);
		}
	}
}

void UComandTableManager::ChangeCommandTable(EWeaponType weapon)
{
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
	EnableAction = FindEnableAction("Base");
	SetCurrentCommandName("Base");
}

void UComandTableManager::SetCurrentCommandName(FString commandName) 
{
	CurrentCommandName = commandName;
	if (CurrentCommands == nullptr) {
		CurrentCommands = &TotalCommands[EWeaponType::E_NOWEAPON];
	}
	EnableAction = CurrentCommands->Find(CurrentCommandName)->EnableChainAction;

	TArray<FChainAction> Result;
	for (auto action : EnableAction) {
		Result.Add(FindAction(action));
	}

	NewActionChainDel.Execute(Result);
}
bool UComandTableManager::ValidCurrentCommandName() const 
{
	return (CurrentCommands->Num() > 0);	
}
FChainAction UComandTableManager::FindAction(FString actionName) 
{
	return *CurrentCommands->Find(actionName);
}