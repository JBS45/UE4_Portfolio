// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BaseMonster.h"
#include "NavigationSystem.h"
#include "Engine/DataTable.h"
#include "Blueprint/AiBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


const FName AMonsterAIController::TargetDistanceKey(TEXT("TargetDistance"));
const FName AMonsterAIController::TargetPlayerKey(TEXT("TargetPlayer"));
const FName AMonsterAIController::TargetLocationKey(TEXT("TargetLocation"));
const FName AMonsterAIController::IsFoundPlayerKey(TEXT("IsFoundPlayer"));
const FName AMonsterAIController::CurrentMonsterState(TEXT("MonsterState"));
const FName AMonsterAIController::PreMonsterState(TEXT("PreMonsterState"));
const FName AMonsterAIController::RandIntKey(TEXT("RandInt"));

AMonsterAIController::AMonsterAIController() {
	static ConstructorHelpers::FObjectFinder<UDataTable> REFDATA(TEXT("DataTable'/Game/DataTable/Monster/MonsterAIRef.MonsterAIRef'"));

	if (REFDATA.Succeeded()) {
		DataRef = REFDATA.Object;
	}
	/*static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Blueprints/C++/Assistant/BB_Assistant.BB_Assistant'"));

	if (BBObject.Succeeded()) {
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Blueprints/C++/Assistant/BT_Assistant.BT_Assistant'"));

	if (BTObject.Succeeded()) {
		BTAsset = BTObject.Object;
	}*/
}
void AMonsterAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	CurrnetMonster = Cast<ABaseMonster>(InPawn);
	SetUpData(CurrnetMonster);
}
void AMonsterAIController::Tick(float DeltaTime) {
	if (UseBlackboard(BBAsset, Blackboard)) {
		Blackboard->SetValueAsBool(IsFoundPlayerKey, CurrnetMonster->GetIsFoundPlayer());
	}
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
void AMonsterAIController::SetUpData(ABaseMonster* monster) {
	static const FString ContextString(TEXT("Monster AI Set Up data"));
	auto result = DataRef->FindRow<FMonsterAIData>(FName(monster->GetMonsterName()), ContextString, true);
	BBAsset = LoadObject<UBlackboardData>(NULL, *(result->BBRef), NULL, LOAD_None, NULL);
	BTAsset = LoadObject<UBehaviorTree>(NULL, *(result->BTRef), NULL, LOAD_None, NULL);

	if (UseBlackboard(BBAsset, Blackboard)) {
		if (!RunBehaviorTree(BTAsset)) {
			TESTLOG(Error, TEXT("AIController couldn't run behavior Tree!"));
		}
		else {
			TESTLOG(Warning, TEXT("AIController run behavior Tree!"));
		}
	}
	CurrnetMonster->ChangeMonsterState(EMonsterStateType::E_IDLE);
	Blackboard->SetValueAsEnum(CurrentMonsterState, (uint8)(CurrnetMonster->GetMonsterState()));
	Blackboard->SetValueAsEnum(CurrentMonsterState, (uint8)(EMonsterStateType::E_NONE));
}

void AMonsterAIController::BeginPlay() {
	Super::BeginPlay();
}
void AMonsterAIController::SetTarget(UObject* target) {
	Blackboard->SetValueAsObject(TargetPlayerKey, target);
}