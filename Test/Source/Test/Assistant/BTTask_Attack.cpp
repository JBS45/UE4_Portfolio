// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "BaseAssistant.h"
#include "AssistantAIController.h"

UBTTask_Attack::UBTTask_Attack() {
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseAssistant>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}
	if (ControllingPawn->BaseAttack()) {
		IsAttacking = true;
		ControllingPawn->OnAttackEnd.AddLambda([this]()->void {IsAttacking = false; });
		return EBTNodeResult::InProgress;

	}
	return EBTNodeResult::Failed;
}
void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}