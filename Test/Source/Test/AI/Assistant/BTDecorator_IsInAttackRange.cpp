// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Assistant/AssistantAIController.h"
#include "../../Assistant/BaseAssistant.h"
#include "../../Monster/BaseMonster.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange() {
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseAssistant>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return false;


	auto Target = Cast<ABaseMonster>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAssistantAIController::TargetMonsterKey));
	if (Target == nullptr) return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetAttackRange());
	return bResult;
}