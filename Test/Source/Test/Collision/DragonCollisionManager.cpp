// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonCollisionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/Skeleton.h"

UDragonCollisionManager::UDragonCollisionManager() {

}
void UDragonCollisionManager::BeginPlay()
{
	Super::BeginPlay();
}

void UDragonCollisionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDragonCollisionManager::InitHitBox(UDataTable* data, USkeletalMeshComponent* mesh)
{
	TArray<FName> Bones;

	TESTLOG(Warning, TEXT("DragonCollison"));



	DataTable = data;
	auto Names = DataTable->GetRowNames();
	for (auto name : Names) {
		auto row = DataTable->FindRow<FMonsterParts>(name, FString(TEXT("Init Hit box Parts data")));

		TArray<FName> Parts;
		SearchChild(mesh, row->PartRootBoneName, Parts, Bones);
		if (HitBox.Contains(row->Part)) {
			HitBox[row->Part]->GetBones().Append(Parts);
		}
		else {
			auto CollisionPart = NewObject<UHitCollisionPart>();
			CollisionPart->SetUpData(*row, Parts);

			HitBox.Add(row->Part, CollisionPart);
		}
	}
		
}
bool UDragonCollisionManager::ReceiveDamage(const FHitResult& hit, const FName socketName,float dmg, int32& outDmage, bool& weak) {
	for (auto element : HitBox) {
		int32 TempDamage = 0;
		bool IsBroken = false;
		//Success Attack
		if (element.Value->CheckGetDamageThisPart(hit, socketName, dmg, TempDamage, IsBroken, element.Key, weak)) {

			outDmage = TempDamage;

			return true;
		}
		outDmage = TempDamage;

	}
	return false;
}