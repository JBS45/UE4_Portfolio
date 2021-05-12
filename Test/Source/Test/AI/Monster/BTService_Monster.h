// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Test.h"
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
class TEST_API UBTService_SearchForMonster : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_SearchForMonster();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
UCLASS()
class TEST_API UBTService_CheckCanChangeBattle : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckCanChangeBattle();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	float Timer;
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

UCLASS()
class TEST_API UBTService_CheckTargetDistance : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckTargetDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, Category = "MaxDistance", meta = (AllowPrivateAccess = "true"))
		float MaxChaseDistance;
};


UCLASS()
class TEST_API UBTService_DecideForward : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_DecideForward();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};