// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UBaseCharAnimInstance::UBaseCharAnimInstance() {

	IsDrawWeapon = false;
	CharSpeed = 0.0f;
	IsRolling = false;
	IsInAir = false;
	CanAttack = true;

	AnimTable = CreateDefaultSubobject<UActionAnimManager>(TEXT("AnimTable"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeAnim(TEXT("AnimMontage'/Game/Blueprints/Animations/Dual/Evade.Evade'"));
	if (EvadeAnim.Succeeded())
	{
		Evade = EvadeAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ExhaustAnim(TEXT("AnimMontage'/Game/Blueprints/Animations/EXHAUST_Montage.EXHAUST_Montage'"));
	if (ExhaustAnim.Succeeded())
	{
		Exhaust = ExhaustAnim.Object;
	}
}
void UBaseCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		float Dist = Pawn->GetVelocity().Size();
		FVector VelocityXY = FVector(Pawn->GetVelocity().X, Pawn->GetVelocity().Y, 0.0f);
		VelocityXY.Normalize();
		VelocityXY*=Dist;
		CharSpeed = VelocityXY.Size();
		auto Character = Cast<ABaseCharacter>(Pawn);

		if (Character) {
			IsInAir = Character->GetMovementComponent()->IsFalling();
			IsDrawWeapon = Character->GetIsDrawWeapon();
			CurrentWeapon = Character->GetWeaponState();
		}
	}
	
}
void UBaseCharAnimInstance::PlayEvade()
{
	Montage_Play(Evade, 1.0f);
}
void UBaseCharAnimInstance::PlayExhaust()
{
	Montage_Play(Exhaust, 1.0f);
}

void UBaseCharAnimInstance::ExchangeWeapon(EWeaponType type) {
	if (CurrentWeapon == type) return;

	CurrentWeapon = type;
	TESTLOG(Warning, TEXT("Change Weapon : %d"),(int32)CurrentWeapon);
	//AnimTable->SetWeaponAnimDT(CurrentWeapon);
}
void UBaseCharAnimInstance::PlayAnimMontage(UAnimMontage* montage) {
	if (montage!=nullptr) {
		TESTLOG(Warning, TEXT("%s"),*(montage->GetName()));
		Montage_Play(montage, 1.0f);
	}
}