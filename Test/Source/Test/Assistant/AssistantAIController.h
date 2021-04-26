// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "AIController.h"
#include "AssistantAIController.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API AAssistantAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAssistantAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
		class ABaseAssistant* CurrnetAssist;

	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

public:
	static const FName PlayerDistanceKey;
	static const FName TargetMonsterKey;
	static const FName TargetLocationKey;
	static const FName IsBattleStateKey;
	static const FName CanActionKey;
};
