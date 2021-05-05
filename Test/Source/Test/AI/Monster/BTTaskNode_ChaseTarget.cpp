// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChaseTarget.h"
#include "../../Monster/MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Monster/BaseMonster.h"
#include "../../Monster/MonsterArea.h"
#include "../../Player/BaseCharacter.h"
#include "NavigationSystem.h"
#include "Tasks/AITask_MoveTo.h"


UBTTaskNode_ChaseTarget::UBTTaskNode_ChaseTarget() {
	NodeName = TEXT("ChaseTarget");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_ChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	
	ControllingPawn->GetMonsterController()->MoveToActor(Target, AcceptableRadius);

	bBegin = true;

	return EBTNodeResult::InProgress;
}
void UBTTaskNode_ChaseTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	if (!bBegin) return;

	if (OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AMonsterAIController::TargetDistanceKey)<= AcceptableRadius) {
		TESTLOG(Warning, TEXT("Wow"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	if (Target == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
