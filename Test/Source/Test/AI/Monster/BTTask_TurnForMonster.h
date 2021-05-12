// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Test.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnForMonster.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTTask_TurnForMonster : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_TurnForMonster();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, Category = "Rotate", meta = (AllowPrivateAccess = "true"))
		float LerpSpped;
		FRotator Dest;
		FVector TargetVector;
private:
	FRotator FindRotation(class ABaseMonster* ControllingPawn);
};
