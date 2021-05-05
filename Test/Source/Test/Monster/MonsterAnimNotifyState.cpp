// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimNotifyState.h"
#include "BaseMonster.h"
#include "../Player/BaseCharacter.h"
#include "../Collision/HitCollisionManager.h"
#include "../Components/DamageInterface.h"
#include "DrawDebugHelpers.h"


FString UMonsterMeleeAttackUnit::GetNotifyName_Implementation() const
{
	return L"Monster Attack Enable";
}

void UMonsterMeleeAttackUnit::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Monster->PartOverlapOn(AttackEnablePart);
		Monster->SetMeleeDamage(DamageRate, KnockBackDistance, DamageType, AttackEnablePart);
	}


}
void UMonsterMeleeAttackUnit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
}
void UMonsterMeleeAttackUnit::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	auto ControllingMonster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (IsValid(ControllingMonster)) {
		ControllingMonster->PartOverlapOff();
	}
}

