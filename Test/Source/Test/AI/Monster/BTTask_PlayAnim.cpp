// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlayAnim.h"
#include "../../Monster/BaseMonster.h"
#include "../../Monster/MonsterAIController.h"
#include "../../Monster/MonsterAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_PlayAnim::UBTTask_PlayAnim() {
	NodeName = TEXT("PlayAnim");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}
	ControllingPawn->GetAnimInst()->PlayAnimation(Anim, 1.0f);
	IsBegin = true;

	return EBTNodeResult::InProgress;
}
void UBTTask_PlayAnim::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	if (!IsBegin) return;

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!ControllingPawn->GetAnimInst()->Montage_IsPlaying(Anim)) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}