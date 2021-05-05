// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "Components/ActorComponent.h"
#include "MovementManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_API UMovementManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovementManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FVoidDelegateBool SetIsSprintForStat;
private:
	const float BasicSpeed = 800.0f;
	const float RunSpeed = 1600.0f;
	const float ExhuastSpeed = 200.0f;
	float SpeedRate = 1.0f;

	EMovementState CurrentMovementState;
	UCharacterMovementComponent* CharMovement;
public:
	inline void SetSpeedRate(float value) { SpeedRate = value; }
	inline EMovementState GetMoveState() { return CurrentMovementState; }

	void SetCharacterMovement(UCharacterMovementComponent* comp);
	void SetMovementState(EMovementState state);
};
