// Fill out your copyright notice in the Description page of Project Settings.


#include "EnableAttackUnit.h"
#include "../BaseWeapon.h"
#include "BaseCharacter.h"
#include "BasePlayerController.h"

FString UEnableAttackUnit::GetNotifyName_Implementation() const {
	return L"Attack Enable";
}

void UEnableAttackUnit::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		switch (WeaponHand) {
			case EHandDamageRate::E_LEFT:
				PlayerCharater->WeaponLeftOverlapOn();
				PlayerCharater->GetLeftHand()->SetDamageRate(DamageRate);
				PlayerCharater->GetLeftHand()->TrailOn((ETrailWidthMode)Type, Value);
				break;
			case EHandDamageRate::E_RIGHT:
				PlayerCharater->WeaponRightOverlapOn();
				PlayerCharater->GetRightHand()->SetDamageRate(DamageRate);
				PlayerCharater->GetRightHand()->TrailOn((ETrailWidthMode)Type, Value);
				break;
		}
	}

}

void UEnableAttackUnit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UEnableAttackUnit::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		switch (WeaponHand) {
		case EHandDamageRate::E_LEFT:
			PlayerCharater->WeaponLeftOverlapOff();
			PlayerCharater->GetLeftHand()->TrailOff();
			break;
		case EHandDamageRate::E_RIGHT:
			PlayerCharater->WeaponRightOverlapOff();
			PlayerCharater->GetRightHand()->TrailOff();
			break;
		}
	}
}



FString UEvadeActionUnit::GetNotifyName_Implementation() const {
	return L"Evade";
}

void UEvadeActionUnit::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void UEvadeActionUnit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UEvadeActionUnit::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

FString UChangeCharacterState::GetNotifyName_Implementation() const {
	return L"ChangeState";
}

void UChangeCharacterState::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Player = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(Player)) {
		auto Controller = Cast<ABasePlayerController>(Player->GetController());
		Controller->ChangeStateDel.Broadcast(NextCharacterState);
	}
}
FString UCheckInAir::GetNotifyName_Implementation() const {
	return L"CheckInAir";
}

void UCheckInAir::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Player = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(Player)) {
		if (!Player->GetCharacterMovement()->IsFalling()) {
			Player->GetAnimInst()->Montage_SetNextSection(CurrentSection, NextSection);
		}
	}
}