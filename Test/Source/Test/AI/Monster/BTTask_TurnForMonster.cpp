// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnForMonster.h"
#include "../../Monster/MonsterAIController.h"
#include "../../Monster/BaseMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Monster/MonsterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_TurnForMonster::UBTTask_TurnForMonster() {
	NodeName = TEXT("Turn");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TurnForMonster::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	if (Target == nullptr) {
		return EBTNodeResult::Failed;
	}
	
	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Normalize();
	


	float dot = FVector::DotProduct(ControllingPawn->GetActorRightVector(), LookVector);

	//right
	if (dot >= 0) {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(1.0f);
	}
	//Left
	else {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(-1.0f);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_TurnForMonster::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	if (Target == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Normalize();

	float dot = FVector::DotProduct(ControllingPawn->GetActorForwardVector(), LookVector);

	auto Goal = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorForwardVector(), LookVector);
	ControllingPawn->SetActorRelativeRotation(UKismetMathLibrary::RInterpTo(ControllingPawn->GetActorRotation(), Goal, DeltaSeconds, 1.5f));

	auto angle = UKismetMathLibrary::RadiansToDegrees(FMath::Acos(dot));
	

	if (angle <= 3) {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(0.0f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}