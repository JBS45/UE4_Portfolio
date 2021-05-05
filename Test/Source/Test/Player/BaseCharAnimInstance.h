// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "Animation/AnimInstance.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ECharacterState CurrentState = ECharacterState::E_NONE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsBattle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float CharSpeed;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Evade", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Evade;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Evade", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* KnockBack;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attacks", meta = (AllowPrivateAccess = "true"))
		TArray<class UAnimMontage*> LeftAttackDualAnims;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool CanAttack;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> SkillAttackDualAnims;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		EWeaponType CurrentWeapon;

private:
	float MakeCharSpeed(APawn* pawn);
public:
	void ExchangeWeapon(EWeaponType type);
	void PlayAnimMontage(UAnimMontage* montage);
	void PlayEvade();
	void PlayKnockBack();

	void CharacterChangeState(ECharacterState state);
	void ChangeWeapon(EWeaponType type);
};
