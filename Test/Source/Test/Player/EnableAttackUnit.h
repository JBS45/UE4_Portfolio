// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnableAttackUnit.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EHandDamageRate : uint8 {
	E_LEFT = 0 UMETA(DisplayName = "LEFT"),
	E_RIGHT = 1 UMETA(DisplayName = "RIGHT")
};
UENUM(BlueprintType)
enum class ETrailModeSelect : uint8 {
	E_CENTER = 0 UMETA(DisplayName = "CENTER"),
	E_FIRST = 1 UMETA(DisplayName = "FIRST"),
	E_SECOND = 2 UMETA(DisplayName = "SECOND"),
};

UCLASS()
class TEST_API UEnableAttackUnit : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
private:
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		EHandDamageRate WeaponHand;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float DamageRate = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Trail", meta = (AllowPrivateAccess = "true"))
		ETrailModeSelect Type;
	UPROPERTY(EditAnywhere, Category = "Trail", meta = (AllowPrivateAccess = "true"))
		float Value = 1.0f;
};

UCLASS()
class TEST_API UEvadeActionUnit : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS()
class TEST_API UChangeCharacterState : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
private:
	UPROPERTY(EditAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ECharacterState NextCharacterState;
};

UCLASS()
class TEST_API UCheckInAir : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
private:
	UPROPERTY(EditAnywhere, Category = "NextSection", meta = (AllowPrivateAccess = "true"))
		FName CurrentSection;
	UPROPERTY(EditAnywhere, Category = "NextSection", meta = (AllowPrivateAccess = "true"))
		FName NextSection;
};
