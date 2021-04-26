// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Test.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "BaseEnum.h"
#include "BaseStatus.generated.h"


USTRUCT(BlueprintType)
struct TEST_API FBaseStatus
{
	GENERATED_USTRUCT_BODY()
public:

	float Hp;
	float Stamina;
	float Dmg;
	float Def;
	float CriticalRate;

public:
	const FBaseStatus operator+(const FBaseStatus& rhs);
	FBaseStatus& operator+= (const FBaseStatus& rhs);
	
};
USTRUCT(BlueprintType)
struct TEST_API FEquipmentStatus
{
	GENERATED_USTRUCT_BODY()
public:
	bool IsEmpty = true;
	uint8 ItemID;

	EEquipmentState EquipType;
	EWeaponType WeaponType;
	FBaseStatus StatusData;
};


USTRUCT(BlueprintType)
struct TEST_API FBaseCharacterStatus
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float MaxHp;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float CurrentHp;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float HpRegen;

	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float CurrentStamina;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float StaminaRegen;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float StaminaConsumeSprint = 2.0f;

	float EvadeUseStaminaAmount = 20.0f;

	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float Dmg;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float Def;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float CriticalRate;

	UPROPERTY(VisibleAnywhere, Category = "CharacterStat")
		FBaseStatus CharacterStatus;
	UPROPERTY(VisibleAnywhere, Category = "EquipSlot")
		TMap<EEquipmentState, FEquipmentStatus> Slot;

};

USTRUCT(BlueprintType)
struct TEST_API FHitBoxInfo
{
	GENERATED_USTRUCT_BODY()
public:

	FVector BoxSize;
	FVector BoxPos;

};

USTRUCT(BlueprintType)
struct TEST_API FBaseMonsterStatus
{
	GENERATED_USTRUCT_BODY()
public:

	float MaxHp;
	float Dmg;
	float Def;

	TMap<FString, float> AccumulateDamageForWeakpoint;
	TMap<FString, float> LimitDamageWeakpoint;
}; 
USTRUCT(BlueprintType)
struct TEST_API FAssistSkill : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Animation;
public:
	bool IsReady = true;
	float CurrentLeftTime =0.0f;
};

USTRUCT(BlueprintType)
struct TEST_API FMonsterAIData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseMonster> MonsterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString BBRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString BTRef;
};