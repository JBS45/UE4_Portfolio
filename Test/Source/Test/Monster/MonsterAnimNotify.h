// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MonsterAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UMonsterRoar : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS()
class TEST_API UMonsterShootFireBall : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};


UCLASS()
class TEST_API UEndPlayAnim : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};