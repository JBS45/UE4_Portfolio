// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "BaseMonster.h"

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