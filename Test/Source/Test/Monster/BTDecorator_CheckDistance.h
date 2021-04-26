// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTDecorator_CheckDistance : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckDistance();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
