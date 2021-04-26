// Fill out your copyright notice in the Description page of Project Settings.


#include "AssistantInstance.h"
#include "BaseAssistant.h"

UAssistantInstance::UAssistantInstance() {
	CharSpeed = 0.0f;
}

void UAssistantInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		float Dist = Pawn->GetVelocity().Size();
		FVector VelocityXY = FVector(Pawn->GetVelocity().X, Pawn->GetVelocity().Y, 0.0f);
		VelocityXY.Normalize();
		VelocityXY *= Dist;
		CharSpeed = VelocityXY.Size();
		auto Character = Cast<ABaseAssistant>(Pawn);

		if (Character) {

		}
	}
}
void UAssistantInstance::PlayAnimMontage(UAnimMontage* montage) {
	if (montage != nullptr) {
		Montage_Play(montage, 1.5f);
	}
}