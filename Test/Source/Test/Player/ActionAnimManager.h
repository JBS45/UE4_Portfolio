// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../WeaponAnimTable.h"
#include "ActionAnimManager.generated.h"


/**
 * 
 */
UCLASS()
class TEST_API UActionAnimManager : public UObject
{
	GENERATED_BODY()
private:
	TArray<class UDataTable*> WeaponActionTables;
	UDataTable* CurrentAnimDT;
public:
	UActionAnimManager();
	~UActionAnimManager();

	void SetWeaponAnimation(const FString ref);
	void SetWeaponAnimDT(const EWeaponType Type);

	UAnimMontage* SelectPlayMontage(int32 ComboCount);//KeyInput
};
