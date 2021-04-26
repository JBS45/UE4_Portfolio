// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Test.h"
#include "BaseEnum.h"
#include "Engine/DataTable.h"
#include "WeaponAnimTable.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8 {
	E_DRAWWEAPON = 0 UMETA(DisplayName = "DRAW"),
	E_LEFTATTACK UMETA(DisplayName = "LEFTATTACK"),
	E_RIGHTATTACK UMETA(DisplayName = "RIGHTATTACK"),
	E_DOUBLEATTACK UMETA(DisplayName = "DOUBLEATTACK"),
	E_EVADE UMETA(DisplayName = "EVADE"),
	E_HIT UMETA(DisplayName = "HIT"),
	E_KNOCKBACK UMETA(DisplayName = "KNOCKBACK"),
	E_DOWN UMETA(DisplayName = "DOWN"),
	E_DETAH UMETA(DisplayName = "DETAH"),
};

USTRUCT(BlueprintType)
struct FWeaponAnimTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class UAnimMontage*> Montage;

};
