// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseStatus.h"
#include "../Player/BaseCharacter.h"
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
		TMap<EMonsterState, FMonsterAI> AITable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* CurrentBT;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBlackboardData* CurrentBB;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		EMonsterState CurrentState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		int32 BrokenPart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target", meta = (AllowPrivateAccess = "true"))
		ABaseCharacter* Target;

	UPROPERTY()
		class UDataTable* DataRef;
private:
	bool IsFoundPlayer = false;
public:
	static const FName TargetDistanceKey;
	static const FName TargetPlayerKey;
	static const FName TargetLocationKey;
	static const FName IsAnimPlaying;
	static const FName CurrentMonsterState;
	static const FName PreMonsterState;
	static const FName RandIntKey;
	static const FName IsFlying;
public:
	void SetUpData(class ABaseMonster* monster);
	void SetTarget(ABaseCharacter* target);
	void SetBehaviorTree(EMonsterState state);
	void ChangeMonsterState(EMonsterState state);
	EMonsterState GetCurrentState() { return CurrentState; }
	ABaseCharacter* GetTarget();
	bool GetIsFoundPlayer() { return IsFoundPlayer; }
	void SetBrokenState(EMonsterPartsType brokenPart);
	//void SetUpPatrolPoints(TArray<class ATargetPoint*> arr);
};
