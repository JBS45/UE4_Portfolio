// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ChaseTarget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBTTaskNode_ChaseTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_ChaseTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, Category = "Node", meta = (AllowPrivateAccess = "true"))
		float AcceptableRadius = 30.0f;

	class ABaseCharacter* Target;
	bool bBegin = false;
};

UCLASS()
class TEST_API UBTTaskNode_ChaseFail : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_ChaseFail();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, Category = "Node", meta = (AllowPrivateAccess = "true"))
		float AcceptableRadius = 30.0f;

	class ABaseCharacter* Target;
	bool bBegin = false;
};