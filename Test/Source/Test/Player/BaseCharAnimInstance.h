// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "Animation/AnimInstance.h"
#include "ActionAnimManager.h"
#include "BaseCharAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBaseCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBaseCharAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool IsDrawWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float CharSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool IsRolling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool IsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float PlayRate;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Evade", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Evade;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Evade", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Exhaust;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
		TArray<class UAnimMontage*> LeftAttackDualAnims;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
		int32 ComboCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool CanAttack;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> SkillAttackDualAnims;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		EWeaponType CurrentWeapon;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimationTable", meta = (AllowPrivateAccess = "true"))
		UActionAnimManager* AnimTable;

private:

public:
	void ExchangeWeapon(EWeaponType type);
	void PlayAnimMontage(UAnimMontage* montage);
	void PlayEvade();
	void PlayExhaust();
	UActionAnimManager* GetAnimManager() { return AnimTable; }

};
