// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementManager.h"

// Sets default values for this component's properties
UMovementManager::UMovementManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// ...
}


// Called when the game starts
void UMovementManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
void UMovementManager::SetCharacterMovement(UCharacterMovementComponent* comp) {
	CharMovement = comp;
	CharMovement->bOrientRotationToMovement = true;
	CharMovement->RotationRate = FRotator(0, 540, 0);
	CharMovement->MaxAcceleration = RunSpeed;
	CharMovement->MaxWalkSpeed = BasicSpeed;
}
void UMovementManager::SetMovementState(EMovementState state) {
	if (CurrentMovementState == state) return;
	CurrentMovementState = state;

	switch (CurrentMovementState) {
	case EMovementState::E_IDLE:
		CharMovement->MaxWalkSpeed = BasicSpeed * SpeedRate;
		SetIsSprintForStat.ExecuteIfBound(false);
		break;
	case EMovementState::E_SPRINT:
		CharMovement->MaxWalkSpeed = RunSpeed * SpeedRate;
		SetIsSprintForStat.ExecuteIfBound(true);
		break;
	case EMovementState::E_EXHAUST:
		CharMovement->MaxWalkSpeed = ExhuastSpeed * SpeedRate;
		SetIsSprintForStat.ExecuteIfBound(false);
		break;
	}
}

