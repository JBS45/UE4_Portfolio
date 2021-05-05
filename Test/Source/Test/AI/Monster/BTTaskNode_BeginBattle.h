// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Test.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_BeginBattle.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTTaskNode_BeginBattle : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_BeginBattle();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
