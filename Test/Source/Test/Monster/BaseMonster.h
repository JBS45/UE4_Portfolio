// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "GameFramework/Character.h"
#include "../Components/DetectComponent.h"
#include "BaseMonster.generated.h"

UCLASS()
class TEST_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetUpMonsterStatus() PURE_VIRTUAL(ABaseMonster::SetUpMonsterStatus);

	virtual FString GetMonsterName() { return Name; }
	virtual class AMonsterArea* GetActiveArea() { return ActiveArea; }
	virtual void SetActiveArea(class AMonsterArea* area) { ActiveArea = area; }

	virtual void SetPatrolNode(int max)PURE_VIRTUAL(ABaseMonster::SetPatrolNode);
	virtual void NextPatrolNode() PURE_VIRTUAL(ABaseMonster::NextPatrolNode);
	virtual int GetCurrentPatrolNode() PURE_VIRTUAL(ABaseMonster::GetCurrentPatrolNode, return CurrentPatrolNode;);
	virtual TArray<APawn*> GetTargetList() PURE_VIRTUAL(ABaseMonster::GetTargetList, return _Detect->GetTargets(););

	virtual float GetAggroChangeTerm() PURE_VIRTUAL(ABaseMonster::GetAggroChangeTerm, return AggroChangeTerm;);
	virtual float GetAttackRange() PURE_VIRTUAL(ABaseMonster::GetAttackRange, return AttackRange;);
	virtual bool GetIsAlive() PURE_VIRTUAL(ABaseMonster::GetIsAlive, return IsAlive;);
	virtual bool GetIsFoundPlayer() PURE_VIRTUAL(ABaseMonster::GetIsFoundPlayer, return IsFoundPlayer;);
	virtual void SetIsFoundPlayer(bool value) PURE_VIRTUAL(ABaseMonster::SetIsFoundPlayer);
	virtual void SetTarget(ACharacter* target) PURE_VIRTUAL(ABaseMonster::SetTarget);
	virtual EMonsterStateType GetMonsterState() PURE_VIRTUAL(ABaseMonster::GetMonsterState, return MonsterState;);
	virtual void ChangeMonsterState(EMonsterStateType type) PURE_VIRTUAL(ABaseMonster::ChangeMonsterState);


protected:
	class UCapsuleComponent* MoveCollision;
	class UHitCollisionManager* HitBox;
	//class UHitCollisionManager* DamageBox;
	FBaseMonsterStatus MonsterStatus;

	class ACharacter* Target;
	class UMonsterAnimInstance* AnimInst;

	UPROPERTY(VisibleAnywhere, Category = "Detect")
		UDetectComponent* _Detect;
	UPROPERTY(VisibleAnywhere, Category = "Detect")
		float DetectRange = 1000.0f;
	UPROPERTY(VisibleAnywhere)
		class AMonsterArea* ActiveArea;
	UPROPERTY(VisibleAnywhere)
		EMonsterStateType MonsterState;

	FString Name;

	bool IsFoundPlayer = false;
	bool IsAlive = true;

	int MaxPatrolNode;
	int CurrentPatrolNode;
	UPROPERTY(EditAnywhere)
	float AggroChangeTerm;
	UPROPERTY(EditAnywhere)
	float AttackRange;

	float BasicSpeed = 100.0f;
	float RunSpeed = 200.0f;
};
