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

	virtual bool TakeDamageFromPlayer(const FHitResult& hit, const FName socketName, float dmg, int32& outDmage,bool& IsWeak) override;
	virtual void ChangeMonsterState(EMonsterState state) override;
	virtual bool MonsterInit(FMonsterStatus status) override;
	virtual void Dead() override;
public:

	void NotifyArea(ACharacter* target);
private:
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Root;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float CurrentHp;

};

