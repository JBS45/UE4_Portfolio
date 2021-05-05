// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimNotify.h"
#include "BaseMonster.h"
#include "BossDragon.h"
#include "MonsterAIController.h"
#include "../Player/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"



FString UMonsterRoar::GetNotifyName_Implementation() const {
	return L"Roar";
}

void UMonsterRoar::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Monster->Roar();
	}
}

FString UMonsterShootFireBall::GetNotifyName_Implementation() const {
	return L"Fire";
}

void UMonsterShootFireBall::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<ABossDragon>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Monster->ShootFireBall();
	}
}

FString UEndPlayAnim::GetNotifyName_Implementation() const {
	return L"EndAnim";
}
void UEndPlayAnim::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (Monster == nullptr) {
		return;
	}
	auto MonsterController = Cast<AMonsterAIController>(Monster);
	if (IsValid(MonsterController)) {
		MonsterController->GetBlackboardComponent()->SetValueAsBool(AMonsterAIController::IsAnimPlaying, false);
		//MonsterController
	}
}
