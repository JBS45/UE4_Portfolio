// Fill out your copyright notice in the Description page of Project Settings.


#include "AssistantAIController.h"
#include "BaseAssistant.h"
#include "NavigationSystem.h"
#include "Blueprint/AiBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


const FName AAssistantAIController::PlayerDistanceKey(TEXT("PlayerDistance"));
const FName AAssistantAIController::TargetMonsterKey(TEXT("TargetMonster"));
const FName AAssistantAIController::TargetLocationKey(TEXT("TargetLocation"));
const FName AAssistantAIController::IsBattleStateKey(TEXT("IsBattleState"));
const FName AAssistantAIController::CanActionKey(TEXT("CanAction"));


AAssistantAIController::AAssistantAIController() {

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Blueprints/C++/Assistant/BB_Assistant.BB_Assistant'"));

	if (BBObject.Succeeded()) {
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Blueprints/C++/Assistant/BT_Assistant.BT_Assistant'"));

	if (BTObject.Succeeded()) {
		BTAsset = BTObject.Object;
	}
}
void AAssistantAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard)) {
		if (!RunBehaviorTree(BTAsset)) {
			TESTLOG(Error, TEXT("AIController couldn't run behavior Tree!"));
		}
	}
	CurrnetAssist = Cast<ABaseAssistant>(InPawn);

}
void AAssistantAIController::Tick(float DeltaTime) {
	if (UseBlackboard(BBAsset, Blackboard)) {
		Blackboard->SetValueAsFloat(PlayerDistanceKey, CurrnetAssist->PlayerDistance());
		Blackboard->SetValueAsBool(IsBattleStateKey, CurrnetAssist->GetIsBattle());
		Blackboard->SetValueAsBool(CanActionKey, CurrnetAssist->GetCanAction());
	}
}

void AAssistantAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
