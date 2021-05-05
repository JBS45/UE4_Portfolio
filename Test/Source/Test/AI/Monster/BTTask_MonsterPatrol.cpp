// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MonsterPatrol.h"
#include "../../Monster/MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Monster/BaseMonster.h"
#include "../../Monster/MonsterArea.h"
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

	auto PatrolPoint = ControllingPawn->GetActiveArea()->GetPatrolPoint();
	int32 Length = PatrolPoint.Num();
	if (Length == 0) {
		return EBTNodeResult::Failed;
	}
	
	int32 Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt("NextPatrolIndex");
	if (Index >= Length) {
		Index = 0;
	}


	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) {
		return EBTNodeResult::Failed;
	}
	FNavLocation NextLocation;
	
	if (NavSystem->GetRandomPointInNavigableRadius(PatrolPoint[Index]->GetActorLocation(), 100.0f, NextLocation)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::TargetLocationKey, NextLocation.Location);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt("NextPatrolIndex", Index + 1);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

UBTTask_FindClosePatrolPoint::UBTTask_FindClosePatrolPoint() {
	NodeName = TEXT("FindColsePoint");
}

EBTNodeResult::Type UBTTask_FindClosePatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto PatrolPoint = ControllingPawn->GetActiveArea()->GetPatrolPoint();
	int32 Length = PatrolPoint.Num();
	if (Length == 0) {
		return EBTNodeResult::Failed;
	}

	int32 MinIndex = 0;
	float MinDist = MAX_FLT;
	for (int i = 0; i < Length; ++i) {
		float dist = FVector::Dist(ControllingPawn->GetActorLocation(), PatrolPoint[i]->GetActorLocation());
		if (dist < MinDist) {
			MinDist = dist;
			MinIndex = i;
		}
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) {
		return EBTNodeResult::Failed;
	}
	FNavLocation NextLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(PatrolPoint[MinIndex]->GetActorLocation(), 100.0f, NextLocation)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::TargetLocationKey, NextLocation.Location);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt("NextPatrolIndex", MinIndex+1);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;


}