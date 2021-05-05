// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Monster.h"
#include "../../BaseEnum.h"
#include "../../Monster/MonsterAIController.h"
#include "../../Monster/BaseMonster.h"
#include "../../Monster/MonsterArea.h"
#include "../../Player/BaseCharacter.h"
#include "../../Assistant/BaseAssistant.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_FindTargetForMonster::UBTService_FindTargetForMonster() {
	NodeName = TEXT("FindTarget");
	Interval = 1.0f;
}


void UBTService_FindTargetForMonster::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());
	auto Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (ControllingPawn == nullptr) return;

	auto target = ControllingPawn->GetTarget();
	if (target == nullptr) {
		Controller->SetTarget(nullptr);
		return;
	}

	float dist = FVector::DistXY(target->GetActorLocation(), ControllingPawn->GetActiveArea()->GetActorLocation());
	if (dist <= ControllingPawn->GetActiveArea()->GetAreaRange()) {
		Controller->SetTarget(target);
	}
	else {
		Controller->SetTarget(target);
	}
	TESTLOG(Warning, TEXT("asd"));
}
UBTService_CheckCanChangeBattle::UBTService_CheckCanChangeBattle() {
	NodeName = TEXT("CheckChangeBattle");
	Interval = 1.0f;
}

void UBTService_CheckCanChangeBattle::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());
	auto Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	TESTLOG(Warning, TEXT("awwds"));
	if (ControllingPawn == nullptr) return;

	auto target = ControllingPawn->GetTarget();

	if (Controller->GetIsFoundPlayer()) {
		Timer += DeltaSeconds;
		if (Timer >= 3.0f) {
			Controller->ChangeMonsterState(EMonsterState::E_BATTLE);
		}
	}
	else {
		Timer = 0;
	}
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

	if(!(ControllingPawn->GetActiveArea()->GetIsPlayerInRange())){
		ControllingPawn->GetMonsterController()->ChangeMonsterState(EMonsterState::E_IDLE);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetPlayerKey, nullptr);
	}

}

UBTService_CheckTargetDistance::UBTService_CheckTargetDistance() {
	NodeName = TEXT("CheckPlayerDistance");
	Interval = 0.5f;
}


void UBTService_CheckTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	float Distance = FVector::Dist(Target->GetActorLocation(), ControllingPawn->GetActorLocation());
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMonsterAIController::TargetDistanceKey, Distance);
	
}

UBTService_ChaseTarget::UBTService_ChaseTarget() {
	NodeName = TEXT("ChasePlayer");
	Interval = 0.5f;
}

void UBTService_ChaseTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::TargetLocationKey, Target->GetActorLocation());
}