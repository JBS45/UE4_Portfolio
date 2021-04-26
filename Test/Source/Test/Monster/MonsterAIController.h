// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseStatus.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMonsterAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		class ABaseMonster* CurrnetMonster;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BTAsset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBlackboardData* BBAsset;
	UPROPERTY()
		class UDataTable* DataRef;

public:
	static const FName TargetDistanceKey;
	static const FName TargetPlayerKey;
	static const FName TargetLocationKey;
	static const FName IsFoundPlayerKey;
	static const FName CurrentMonsterState;
	static const FName PreMonsterState;
	static const FName RandIntKey;
public:
	void SetUpData(class ABaseMonster* monster);
	void SetTarget(UObject* target);
};
