// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_BeginBattle.h"
#include "../../Monster/MonsterAIController.h"
#include "../../Monster/Wolf_Monster.h"
#include "../../Monster/MonsterArea.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_BeginBattle::UBTTaskNode_BeginBattle() {
	NodeName = TEXT("Notify");
}

EBTNodeResult::Type UBTTaskNode_BeginBattle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AWolf_Monster>(OwnerComp.GetAIOwner()->GetPawn());

	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto Monsters = ControllingPawn->GetActiveArea()->GetMonsters();
	/*for (auto monster : Monsters) {
		Cast<AMonsterAIController>(monster->GetController())->SetTarget(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
		monster->ChangeMonsterState(EMonsterStateType::E_BATTLE);
	}


	TESTLOG(Warning, TEXT("ROAR"));*/
	return EBTNodeResult::Succeeded;
}