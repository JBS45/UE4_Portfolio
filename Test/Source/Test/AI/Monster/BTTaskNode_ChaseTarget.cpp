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
}

EBTNodeResult::Type UBTTaskNode_ChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}


	Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	
	auto ChaseResult = ControllingPawn->GetMonsterController()->MoveToActor(Target, AcceptableRadius);

	if (ChaseResult != EPathFollowingRequestResult::Failed) {
		TESTLOG(Warning, TEXT("Success"));
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

UBTTaskNode_ChaseFail::UBTTaskNode_ChaseFail() {
	NodeName = TEXT("ChaseFail");
}

EBTNodeResult::Type UBTTaskNode_ChaseFail::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Control = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (Control == nullptr) {
		return EBTNodeResult::Failed;
	}

	Control->ChangeMonsterState(EMonsterState::E_IDLE);

	
	return EBTNodeResult::Succeeded;
}