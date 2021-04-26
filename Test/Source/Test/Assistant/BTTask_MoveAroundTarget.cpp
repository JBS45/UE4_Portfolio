// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveAroundTarget.h"
#include "AssistantAIController.h"
#include "BaseAssistant.h"
#include "../Monster/BaseMonster.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_MoveAroundTarget::UBTTask_MoveAroundTarget() {
	NodeName = TEXT("MoveAround");
}

EBTNodeResult::Type UBTTask_MoveAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseAssistant>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<ABaseMonster>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAssistantAIController::TargetMonsterKey));
	if (Target == nullptr) {
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) {
		return EBTNodeResult::Failed;
	}
	FNavLocation NextLocation;
	NextLocation.Location = Target->GetActorLocation();
	bool Result = false;
	while (FVector::DistXY(Target->GetActorLocation(), NextLocation.Location) <= 200.0f) {
		Result = NavSystem->GetRandomPointInNavigableRadius(Target->GetActorLocation(), 400.0f, NextLocation);
	}

	if (Result) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAssistantAIController::TargetLocationKey, NextLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	
	return EBTNodeResult::Failed;
}