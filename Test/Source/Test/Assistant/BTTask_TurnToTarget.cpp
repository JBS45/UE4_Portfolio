// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "AssistantAIController.h"
#include "BaseAssistant.h"
#include "../Monster/BaseMonster.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget() {
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseAssistant>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<ABaseMonster>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAssistantAIController::TargetMonsterKey));
	if (Target == nullptr) {
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}