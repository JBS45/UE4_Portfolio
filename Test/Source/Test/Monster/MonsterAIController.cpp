// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BaseMonster.h"
#include "NavigationSystem.h"
#include "Engine/DataTable.h"
#include "Blueprint/AiBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"


const FName AMonsterAIController::TargetDistanceKey(TEXT("TargetDistance"));
const FName AMonsterAIController::TargetPlayerKey(TEXT("TargetPlayer"));
const FName AMonsterAIController::TargetLocationKey(TEXT("TargetLocation"));
const FName AMonsterAIController::IsAnimPlaying(TEXT("IsAnimPlaying"));
const FName AMonsterAIController::CurrentMonsterState(TEXT("MonsterState"));
const FName AMonsterAIController::PreMonsterState(TEXT("PreMonsterState"));
const FName AMonsterAIController::RandIntKey(TEXT("RandInt"));
const FName AMonsterAIController::IsFlying(TEXT("IsFlying"));

AMonsterAIController::AMonsterAIController() {
	static ConstructorHelpers::FObjectFinder<UDataTable> REFDATA(TEXT("DataTable'/Game/DataTable/Monster/MonsterAITable.MonsterAITable'"));

	if (REFDATA.Succeeded()) {
		DataRef = REFDATA.Object;
	}
	CurrentState = EMonsterState::E_CREATE;
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

}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
	TESTLOG(Warning, TEXT("UnPossess!"));
}
void AMonsterAIController::SetUpData(ABaseMonster* monster) {
		
	static const FString ContextString(TEXT("Monster AI Set Up data"));
	FMonsterAIData* result = nullptr;
	for (auto name : DataRef->GetRowNames()) {
		result = DataRef->FindRow<FMonsterAIData>(name, ContextString, true);
		if (monster->GetMonsterID() == result->MonsterType.GetDefaultObject()->GetMonsterID()) {
			break;
		}
	}

	for (auto AI : result->MonsterAI) {
		AITable.Add(AI.State, AI);
	}
	ChangeMonsterState(EMonsterState::E_IDLE);
}

void AMonsterAIController::BeginPlay() {
	Super::BeginPlay();
}
void AMonsterAIController::SetTarget(ABaseCharacter* target) {
	Target = target;
	if (Target == nullptr) {
		IsFoundPlayer = false;
	}
	else {
		IsFoundPlayer = true;
	}
}
void AMonsterAIController::SetBehaviorTree(EMonsterState state) {
	CurrentBB = AITable[state].BlackBoard;
	CurrentBT = AITable[state].BehaviorTree;
	
	if (UseBlackboard(CurrentBB, Blackboard)) {
		Blackboard->SetValueAsObject(TargetPlayerKey, Target);
		if (!RunBehaviorTree(CurrentBT)) {
			TESTLOG(Error, TEXT("AIController couldn't run behavior Tree!"));
		}
	}
	TESTLOG(Warning, TEXT("run behavior Tree!"));
}
void AMonsterAIController::ChangeMonsterState(EMonsterState state) {
	if (CurrentState == state) return;
	if (Blackboard != nullptr) {
		Blackboard->SetValueAsEnum(PreMonsterState, (uint8)(CurrentState));
		CurrentState = state;
		Blackboard->SetValueAsEnum(CurrentMonsterState, (uint8)(CurrentState));
	}
	else {
		CurrentState = state;;
	}

	switch (CurrentState) {
		case EMonsterState::E_IDLE:
			SetBehaviorTree(EMonsterState::E_IDLE);
			CurrnetMonster->ChangeMonsterState(EMonsterState::E_IDLE);
			break;
		case EMonsterState::E_BATTLE:
			SetBehaviorTree(EMonsterState::E_BATTLE);
			CurrnetMonster->ChangeMonsterState(EMonsterState::E_BATTLE);
			break;
		case EMonsterState::E_DEAD:
			GetBrainComponent()->StopLogic(TEXT("Monster Is Dead"));
			CurrnetMonster->ChangeMonsterState(EMonsterState::E_DEAD);
			break;
	}
}
ABaseCharacter* AMonsterAIController::GetTarget() {
	return Target;
}
void AMonsterAIController::SetBrokenState(EMonsterPartsType brokenPart) {
	switch (brokenPart)
	{
	case EMonsterPartsType::E_HEAD:
		CurrnetMonster->PartBroken(brokenPart);
		break;
	case EMonsterPartsType::E_BODY:
		break;
	case EMonsterPartsType::E_LEFTHAND:
		break;
	case EMonsterPartsType::E_RIGHTHAND:
		break;
	case EMonsterPartsType::E_WING:
		break;
	case EMonsterPartsType::E_LEFTLEG:
		break;
	case EMonsterPartsType::E_RIGHTLEG:
		break;
	case EMonsterPartsType::E_TAIL:
		CurrnetMonster->PartBroken(brokenPart);
		break;
	}
}

