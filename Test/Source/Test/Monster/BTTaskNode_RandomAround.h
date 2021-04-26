// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RandomAround.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTTaskNode_RandomAround : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_RandomAround();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
