// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "BaseMonster.h"
#include "BossDragon.generated.h"

/**
 * 
 */
class UDecalComponent;
class ADragonFireBall;

UCLASS()
class TEST_API ABossDragon : public ABaseMonster
{
	GENERATED_BODY()
	
public:
	ABossDragon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void ChangeMonsterState(EMonsterState state) override;
	virtual bool MonsterInit(FMonsterStatus status) override;
	virtual void Dead() override;
	virtual void Roar() override;
	virtual void SetBrokenState(EMonsterPartsType brokenPart) override;

public:
	void ChangeGroundToFly();
	virtual void PartBroken(EMonsterPartsType brokenPart) override;
	void ShootFireBall();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsFly;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float FlySpeed;
	UPROPERTY(EditAnywhere, Category = "Decal", meta = (AllowPrivateAccess = "true"))
		TArray<class UDecalComponent*> Wounds;
	UPROPERTY(EditAnywhere, Category = "Decal", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ADragonFireBall> ProjectileType;
private:
	TArray<class ADragonFireBall*> FireBalls;
private:
	ADragonFireBall*  FindFireBall();
};
