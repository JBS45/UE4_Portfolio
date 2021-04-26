// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnForMonster.h"
#include "MonsterAIController.h"
#include "BaseMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnForMonster::UBTTask_TurnForMonster() {
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnForMonster::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	if (Target == nullptr) {
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}