 // Fill out your copyright notice in the Description page of Project Settings.

#include "ActionAnimManager.h"
#include "../LoadAssetStruct.h"


UActionAnimManager::UActionAnimManager()
{
	UDataTable* TmpTable = nullptr;
	static ConstructorHelpers::FObjectFinder<UDataTable> AnimDT(TEXT("DataTable'/Game/DataTable/AnimAssetTable.AnimAssetTable'"));
	if (AnimDT.Succeeded())
	{
		TmpTable =AnimDT.Object;
	}

	if (IsValid(TmpTable)) {
		TArray<FName> RowNames = TmpTable->GetRowNames();

		for (auto name : RowNames)
		{
			TESTLOG(Warning, TEXT("Type : %s"), *(name.ToString()));
			static const FString ContextString(TEXT("Weapon Animation"));
			auto WeaponAnimTableRef = TmpTable->FindRow<FLoadAssetTable>(name, ContextString, true)->reference;
			SetWeaponAnimation(WeaponAnimTableRef);
		}
	}
	
}

UActionAnimManager::~UActionAnimManager()
{
}
void UActionAnimManager::SetWeaponAnimation(const FString ref) {
	TESTLOG(Warning, TEXT("Table : %s"), *(ref));
	ConstructorHelpers::FObjectFinder<UDataTable> WeaponAnimDT(*ref);
	if (WeaponAnimDT.Succeeded())
	{
		WeaponActionTables.Emplace(WeaponAnimDT.Object);
	}
}
void UActionAnimManager::SetWeaponAnimDT(const EWeaponType Type) {
	int32 TypeNum = ((int32)Type);
	TESTLOG(Warning, TEXT("Type : %d , Num : %d"), TypeNum, WeaponActionTables.Num());
	if (TypeNum == (int32)EWeaponType::E_NOWEAPON) {
		CurrentAnimDT = nullptr;
	}
	else {
		CurrentAnimDT = WeaponActionTables[TypeNum];
		TESTLOG(Warning, TEXT("%s"), *(CurrentAnimDT->GetName()));
	}
}

UAnimMontage* UActionAnimManager::SelectPlayMontage(int32 ComboCount) {
	static const FString ContextString = TEXT("Action");
	if (CurrentAnimDT != nullptr) {
		FWeaponAnimTable* TmpTable = CurrentAnimDT->FindRow<FWeaponAnimTable>(FName(TEXT("LeftAttack")), ContextString, true);

		if (TmpTable != nullptr) {
			auto AnimArray = TmpTable->Montage;
			auto tmp = AnimArray[ComboCount];

			TESTLOG(Warning, TEXT("?"));

			if (IsValid(tmp)) {
				return tmp;
			}
		}
	}
	return nullptr;
}