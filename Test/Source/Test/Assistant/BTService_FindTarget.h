// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTService_FindTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
