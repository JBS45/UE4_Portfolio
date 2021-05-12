// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlayAnim.h"
#include "../../Monster/BaseMonster.h"
#include "../../Monster/MonsterAIController.h"
#include "../../Monster/MonsterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
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

UBTTask_PlayAnimToForward::UBTTask_PlayAnimToForward() {
	NodeName = TEXT("PlayAnimForward");
	distance = 200.0f;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayAnimToForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	FVector Forward = ControllingPawn->GetActorForwardVector();

	FVector Dest = ControllingPawn->GetActorLocation() + (Forward * distance);

	ControllingPawn->GetMonsterController()->MoveToLocation(Dest, 10.0f,true,false);
	//ControllingPawn->LaunchCharacter((Forward * distance),true, false);
	ControllingPawn->GetAnimInst()->PlayAnimation(Anim, 1.0f);



	IsBegin = true;

	return EBTNodeResult::InProgress;
}
void UBTTask_PlayAnimToForward::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	if (!IsBegin) return;

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!ControllingPawn->GetAnimInst()->Montage_IsPlaying(Anim)) {
		ControllingPawn->GetCharacterMovement()->StopActiveMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

UBTTask_PlayTwoAnim::UBTTask_PlayTwoAnim() {
	NodeName = TEXT("PlayRotateAnim");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayTwoAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	ControllingPawn->GetAnimInst()->PlayAnimation(Anim, 1.0f);

	//right
	if (Degree >= 0) {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(1.0f);
	}
	//Left
	else {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(-1.0f);
	}

	float time = Anim->GetPlayLength();
	Lerpspeed = Degree / time;
	DestRotation = ControllingPawn->GetActorRotation() + FRotator(0, Degree, 0);



	return EBTNodeResult::InProgress;
}
void UBTTask_PlayTwoAnim::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	if (Target == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	FRotator Temp = ControllingPawn->GetActorRotation(); 
	ControllingPawn->SetActorRelativeRotation(FRotator(Temp.Pitch, Temp.Yaw + (Lerpspeed*DeltaSeconds),Temp.Roll));

	if (!ControllingPawn->GetAnimInst()->Montage_IsPlaying(Anim)) {
		ControllingPawn->GetCharacterMovement()->StopActiveMovement();
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(0.0f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}