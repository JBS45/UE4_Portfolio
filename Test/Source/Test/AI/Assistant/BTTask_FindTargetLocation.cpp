// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindTargetLocation.h"
#include "../../Assistant/AssistantAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "../../Assistant/BaseAssistant.h"
#include "../../Player/BaseCharacter.h"

UBTTask_FindTargetLocation::UBTTask_FindTargetLocation() {
	NodeName = TEXT("FindMaster");
}

EBTNodeResult::Type UBTTask_FindTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseAssistant>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) {
		return EBTNodeResult::Failed;
	}
	FNavLocation NextLocation;

	if (NavSystem->GetRandomPointInNavigableRadius((ControllingPawn->GetMaster())->GetActorLocation(), ControllingPawn->GetFollowRange(), NextLocation)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAssistantAIController::TargetLocationKey, NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}