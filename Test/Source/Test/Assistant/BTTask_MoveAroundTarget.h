// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTTask_MoveAroundTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_MoveAroundTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

