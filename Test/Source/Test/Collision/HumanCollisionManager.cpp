// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanCollisionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Components/CharacterStatusManager.h"
#include "../Player/BaseCharacter.h"

UHumanCollisionManager::UHumanCollisionManager() {

}

void UHumanCollisionManager::BeginPlay()
{
	Super::BeginPlay();
}

void UHumanCollisionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UHumanCollisionManager::InitHitBox(UDataTable* data, USkeletalMeshComponent* mesh)
{
	if (Character == nullptr) return;
	
	FMonsterParts PlayerCollision;
	PlayerCollision.CanDestory = false;
	PlayerCollision.IsWeak = false;
	PlayerCollision.Part = EMonsterPartsType::E_BODY;
	PlayerCollision.DestroyDamage = 0;
	PlayerCollision.Defence = 0;


	auto CollisionPart = NewObject<UHitCollisionPart>();
	CollisionPart->SetUpData(PlayerCollision, Character->GetMesh()->GetAllSocketNames());
	HitBox.Add(PlayerCollision.Part, CollisionPart);

}
bool UHumanCollisionManager::ReceiveDamage(const FHitResult& hit, const FName socketName, float dmg, int32& outDmage, bool& IsWeak) {
	for (auto element : HitBox) {
		int32 TempDamage = 0;
		bool IsBroken = false;
		//Success Attack
		if (element.Value->CheckGetDamageThisPart(hit, socketName, dmg, TempDamage, IsBroken, element.Key, IsWeak)) {

			outDmage = TempDamage;

			return true;
		}
		outDmage = TempDamage;

	}
	return false;
}
void UHumanCollisionManager::SetUpCharacterStatusManager(class ABaseCharacter* character) {
	Character = character;
	StatusManager = Character->GetCharacterStatus();

}