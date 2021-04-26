// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindTarget.h"
#include "AssistantAIController.h"
#include "BaseAssistant.h"
#include "../Player/BaseCharacter.h"
#include "../Monster/BaseMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_FindTarget::UBTService_FindTarget() {
	NodeName = TEXT("FindTarget");
	Interval = 1.0f;
}


void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ABaseAssistant>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;
	/*Current Target is Empty*/
	if (OwnerComp.GetBlackboardComponent()->GetValueAsClass(AAssistantAIController::TargetMonsterKey) == nullptr) {
		/*if Assistant Class have target also the target is Alive*/
		if (ControllingPawn->GetTarget() != nullptr && ControllingPawn->GetTarget()->GetIsAlive()) {
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAssistantAIController::TargetMonsterKey, ControllingPawn->GetTarget());
		}
		/*if Assistant Class dont have target or Target is Dead*/
		else {

			/*Assistant's Detect have Target in Range*/
			if (ControllingPawn->GetTargetList().Num() > 0) {
				TArray<ABaseMonster*> TargetList;
				int RandomNum;
				for (auto target : ControllingPawn->GetTargetList()) {
					auto Temp = Cast<ABaseMonster>(target);
					if (Temp->GetIsAlive()) {
						TargetList.Add(Temp);
					}
				}
				RandomNum = FMath::RandRange(0, TargetList.Num() - 1);

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAssistantAIController::TargetMonsterKey, TargetList[RandomNum]);
			}
			/*Assistant's Detect dont have Target in Range*/
			else {
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(AAssistantAIController::IsBattleStateKey, false);
			}
		}
	}
	else {
		if (!ControllingPawn->GetTarget()->GetIsAlive()) {
			if (ControllingPawn->GetTargetList().Num() > 0) {
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAssistantAIController::TargetMonsterKey, nullptr);
			}
			else {
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(AAssistantAIController::IsBattleStateKey, false);
			}
		}
	}
	
}