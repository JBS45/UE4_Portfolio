// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "BaseMonster.h"
#include "MonsterAIController.h"
#include "../Player/BaseCharacter.h"

UMonsterAnimInstance::UMonsterAnimInstance() {

}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		float Dist = Pawn->GetVelocity().Size();
		FVector VelocityXY = FVector(Pawn->GetVelocity().X, Pawn->GetVelocity().Y, 0.0f);
		VelocityXY.Normalize();
		VelocityXY *= Dist;
		CharSpeed = VelocityXY.Size();
		auto Character = Cast<ABaseMonster>(Pawn);
	}
}

void UMonsterAnimInstance::PlayAnimation(UAnimMontage* montage, float rate) {
	if (montage == nullptr) return;

	if (!Montage_IsPlaying(montage)) {
		Montage_Play(montage, rate);
	}
}
void UMonsterAnimInstance::PlayDown() {

}
void UMonsterAnimInstance::PlayGetUp() {

}

void UMonsterAnimInstance::ChangeMonsterState(EMonsterState state) {
	if (CurrentState == state) return;
	CurrentState = state;
	switch (CurrentState) {
	case EMonsterState::E_IDLE:
		IsBattle = false;
		break;
	case EMonsterState::E_BATTLE:
		IsBattle = true;
		break;
	case EMonsterState::E_RAGE:
		IsBattle = true;
		break;
	case EMonsterState::E_DEAD:
		IsAlive = false;
		break;
	}
}