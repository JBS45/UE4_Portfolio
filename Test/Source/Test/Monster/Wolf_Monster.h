// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "BaseMonster.h"
#include "../BaseStatus.h"
#include "Wolf_Monster.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API AWolf_Monster : public ABaseMonster
{
	GENERATED_BODY()
	
public:
	AWolf_Monster();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetUpMonsterStatus() override;

	void SetArea(class AMonsterArea* area);
	virtual FString GetMonsterName() override;
	virtual class AMonsterArea* GetActiveArea() override;
	virtual void SetActiveArea(class AMonsterArea* area) override;

	virtual void SetPatrolNode(int max) override;
	virtual void NextPatrolNode() override;
	virtual int GetCurrentPatrolNode() override;
	virtual bool GetIsAlive() override;
	virtual bool GetIsFoundPlayer() override;
	virtual void SetIsFoundPlayer(bool value) override;
	virtual float GetAggroChangeTerm() override;
	virtual TArray<APawn*> GetTargetList() override;
	virtual float GetAttackRange() override;
	virtual void SetTarget(ACharacter* target) override;
	virtual EMonsterStateType GetMonsterState();
	virtual void ChangeMonsterState(EMonsterStateType type) override;

	void NotifyArea(ACharacter* target);
private:
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Root;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float CurrentHp;

};

