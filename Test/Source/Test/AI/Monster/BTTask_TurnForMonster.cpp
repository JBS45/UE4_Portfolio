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
	LerpSpped = 20.0f;
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

	TargetVector = FVector((Target->GetActorLocation() - ControllingPawn->GetActorLocation()).X, (Target->GetActorLocation() - ControllingPawn->GetActorLocation()).Y,0);
	TargetVector.Normalize();

	Dest = FindRotation(ControllingPawn);

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

	ControllingPawn->SetActorRelativeRotation(UKismetMathLibrary::RInterpTo(ControllingPawn->GetActorRotation(), Dest, DeltaSeconds, LerpSpped));

	
	auto angle = UKismetMathLibrary::Acos(UKismetMathLibrary::Dot_VectorVector(ControllingPawn->GetActorForwardVector(), TargetVector));
	angle = FMath::RadiansToDegrees(angle);

	if (angle <= 2* LerpSpped) {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(0.0f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else {

		Dest = FindRotation(ControllingPawn);
	}

}
FRotator UBTTask_TurnForMonster::FindRotation(ABaseMonster* ControllingPawn) {

	FRotator Result;
	float dotRight = FVector::DotProduct(ControllingPawn->GetActorRightVector(), TargetVector);

	//right
	if (dotRight >= 0) {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(1.0f);
	}
	//Left
	else {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(-1.0f);
	}


	Result = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorForwardVector(), TargetVector);
	return Result;
}