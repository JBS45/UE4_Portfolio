// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Monster.h"
#include "../BaseEnum.h"
#include "MonsterAIController.h"
#include "BaseMonster.h"
#include "MonsterArea.h"
#include "../Player/BaseCharacter.h"
#include "../Assistant/BaseAssistant.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_FindTargetForMonster::UBTService_FindTargetForMonster() {
	NodeName = TEXT("FindTarget");
	Interval = 3.0f;
}


void UBTService_FindTargetForMonster::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;

	auto list = ControllingPawn->GetTargetList();
	for (auto element : list) {
		auto player = Cast<ABaseCharacter>(element);
		if (IsValid(player)) {
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetPlayerKey, player);
			ControllingPawn->ChangeMonsterState(EMonsterStateType::E_FIND);
		}
	}

}

UBTService_CheckMonsterState::UBTService_CheckMonsterState() {
	NodeName = TEXT("CheckMonsterState");
	Interval = 0.1f;
}


void UBTService_CheckMonsterState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;
	
	auto state = ControllingPawn->GetMonsterState();

	if ((EMonsterStateType)(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(AMonsterAIController::CurrentMonsterState)) == state) return;
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMonsterAIController::PreMonsterState, (uint8)(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(AMonsterAIController::CurrentMonsterState)));
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMonsterAIController::CurrentMonsterState, (uint8)state);

	switch (state) {
		case EMonsterStateType::E_IDLE:
			OwnerComp.RestartTree();
			break;
		case EMonsterStateType::E_FIND:
			ControllingPawn->ChangeMonsterState(EMonsterStateType::E_BATTLE);
			OwnerComp.RestartTree();
			break;
		case EMonsterStateType::E_BATTLE:
			break;
		case EMonsterStateType::E_HIT:
			break;
		case EMonsterStateType::E_DEAD:
			break;
	}
	//OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AMonsterAIController::CurrentMonsterState,(uint8)ControllingPawn->GetMonsterState());
}

UBTService_Random::UBTService_Random() {
	NodeName = TEXT("Random");
	Interval = 1.5f;
}

void UBTService_Random::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;

	int Random = FMath::RandRange(0, 9);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMonsterAIController::RandIntKey, Random);
}

UBTService_CheckTargetInArea::UBTService_CheckTargetInArea() {
	NodeName = TEXT("CheckInArea");
	Interval = 1.5f;
}

void UBTService_CheckTargetInArea::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;

	if(	OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey)!=nullptr &&
		!ControllingPawn->GetActiveArea()->GetIsPlayerInRange()) {
		ControllingPawn->ChangeMonsterState(EMonsterStateType::E_IDLE);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetPlayerKey, nullptr);
	}
}