// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterAIController.h"
#include "BaseMonster.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance() {
	NodeName = TEXT("CanAttack");
}
bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return false;


	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	if (Target == nullptr) return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetAttackRange());
	return bResult;
}