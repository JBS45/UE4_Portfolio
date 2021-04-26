// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RandomAround.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseMonster.h"
#include "MonsterArea.h"
#include "NavigationSystem.h"


UBTTaskNode_RandomAround::UBTTaskNode_RandomAround() {
	NodeName = TEXT("RandomPatrol");
}

EBTNodeResult::Type UBTTaskNode_RandomAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) {
		return EBTNodeResult::Failed;
	}
	FNavLocation NextLocation;


	if (NavSystem->GetRandomPointInNavigableRadius(ControllingPawn->GetActorLocation(), 300.0f, NextLocation)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::TargetLocationKey, NextLocation.Location);
		ControllingPawn->NextPatrolNode();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}