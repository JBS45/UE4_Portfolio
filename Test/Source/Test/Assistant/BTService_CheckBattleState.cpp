// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckBattleState.h"
#include "AssistantAIController.h"
#include "BaseAssistant.h"
#include "../Player/BaseCharacter.h"
#include "../Monster/BaseMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckBattleState::UBTService_CheckBattleState() {
	NodeName = TEXT("CheckBattleState");
	Interval = 5.0f;
}

void UBTService_CheckBattleState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABaseAssistant* ControllingPawn = Cast<ABaseAssistant>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn != nullptr) {
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(AAssistantAIController::IsBattleStateKey)) {
			if (OwnerComp.GetBlackboardComponent()->GetValueAsClass(AAssistantAIController::TargetMonsterKey) == nullptr) {
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(AAssistantAIController::IsBattleStateKey, false);
			}
		}
		else {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AAssistantAIController::IsBattleStateKey, ControllingPawn->GetIsBattle());
		}
	}
}
