// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterPatrol.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseMonster.h"
#include "MonsterArea.h"
#include "NavigationSystem.h"


UBTTask_MonsterPatrol::UBTTask_MonsterPatrol() {
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTask_MonsterPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
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
	
	
	auto PatrolPoint = ControllingPawn->GetActiveArea()->GetPatrolPoint();
	if (NavSystem->GetRandomPointInNavigableRadius(PatrolPoint[ControllingPawn->GetCurrentPatrolNode()]->GetActorLocation(), 150.0f, NextLocation)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::TargetLocationKey, NextLocation.Location);
		ControllingPawn->NextPatrolNode();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}