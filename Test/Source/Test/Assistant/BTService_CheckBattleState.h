// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckBattleState.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTService_CheckBattleState : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckBattleState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
