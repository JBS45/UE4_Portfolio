// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Test.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTTask_FindTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindTargetLocation();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
