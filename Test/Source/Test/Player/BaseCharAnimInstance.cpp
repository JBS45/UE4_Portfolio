// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UBaseCharAnimInstance::UBaseCharAnimInstance() {

	IsBattle = false;
	IsDead = false;
	CharSpeed = 0.0f;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeAnim(TEXT("AnimMontage'/Game/Blueprints/Animations/Dual/Evade.Evade'"));
	if (EvadeAnim.Succeeded())
	{
		Evade = EvadeAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> KnockBackAnim(TEXT("AnimMontage'/Game/Blueprints/Animations/Dual/KnockBack.KnockBack'"));
	if (EvadeAnim.Succeeded())
	{
		KnockBack = KnockBackAnim.Object;
	}

}
void UBaseCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		CharSpeed = MakeCharSpeed(Pawn);
		auto Character = Cast<ABaseCharacter>(Pawn);

		if (Character) {
			CurrentWeapon = Character->GetWeaponState();
		}
	}
	
}
void UBaseCharAnimInstance::PlayEvade()
{
	Montage_Play(Evade, 1.0f);
}
void UBaseCharAnimInstance::PlayKnockBack()
{
	Montage_Play(KnockBack, 1.0f);
}
void UBaseCharAnimInstance::ExchangeWeapon(EWeaponType type) {
	if (CurrentWeapon == type) return;

	CurrentWeapon = type;
	TESTLOG(Warning, TEXT("Change Weapon : %d"),(int32)CurrentWeapon);
}
void UBaseCharAnimInstance::PlayAnimMontage(UAnimMontage* montage) {
	if (montage!=nullptr) {
		TESTLOG(Warning, TEXT("%s"),*(montage->GetName()));
		Montage_Play(montage, 1.0f);
	}
}

void UBaseCharAnimInstance::CharacterChangeState(ECharacterState state) {
	if (CurrentState == state) return;
	CurrentState = state;

	switch (CurrentState) {
	case ECharacterState::E_IDLE:
		IsBattle = false;
		break;
	case ECharacterState::E_BATTLE:
		IsBattle = true;
		break;
	case ECharacterState::E_HIT:
		IsBattle = false;
		TESTLOG(Warning, TEXT("HIT"));
		break;
	case ECharacterState::E_DOWN:
		IsBattle = false;
		IsDown = true;
		break;
	case ECharacterState::E_DEAD:
		TESTLOG(Warning, TEXT("DEAD"));
		IsDead = true;
		break;
	}
}

void UBaseCharAnimInstance::ChangeWeapon(EWeaponType type) {
	if (CurrentWeapon == type) return;

	CurrentWeapon = type;
	TESTLOG(Warning, TEXT("Change Weapon : %d"), (int32)CurrentWeapon);
}

float UBaseCharAnimInstance::MakeCharSpeed(APawn* pawn) {
	float Dist = pawn->GetVelocity().Size();
	FVector VelocityXY = FVector(pawn->GetVelocity().X, pawn->GetVelocity().Y, 0.0f);
	VelocityXY.Normalize();
	VelocityXY *= Dist;
	return VelocityXY.Size();
}