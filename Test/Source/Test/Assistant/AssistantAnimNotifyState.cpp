// Fill out your copyright notice in the Description page of Project Settings.


#include "AssistantAnimNotifyState.h"
#include "BaseAssistant.h"

FString UAssistantAnimNotifyState::GetNotifyName_Implementation() const {
	return L"AssistantNotifyState";
}

void UAssistantAnimNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
}
void UAssistantAnimNotifyState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) {
}
void UAssistantAnimNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
}

FString UAssistantAttackEnd::GetNotifyName_Implementation() const{
	return L"AttackEnd";
}
void UAssistantAttackEnd::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Assistant = Cast<ABaseAssistant>(MeshComp->GetOwner());
	if (IsValid(Assistant)) {
		Assistant->OnAttackMontageEnd();
	}
	
}

FString UAssistantAttackBegin::GetNotifyName_Implementation() const {
	return L"AttackBegin";
}
void UAssistantAttackBegin::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Assistant = Cast<ABaseAssistant>(MeshComp->GetOwner());
	if (IsValid(Assistant)) {
		Assistant->OnAttackMontageBegin();
	}

}