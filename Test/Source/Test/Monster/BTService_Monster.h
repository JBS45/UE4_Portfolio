// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Monster.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTService_FindTargetForMonster : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_FindTargetForMonster();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
UCLASS()
class TEST_API UBTService_CheckMonsterState : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckMonsterState();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
UCLASS()
class TEST_API UBTService_Random : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Random();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

UCLASS()
class TEST_API UBTService_CheckTargetInArea : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckTargetInArea();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};