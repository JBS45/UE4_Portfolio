// Fill out your copyright notice in the Description page of Project Settings.


#include "HitCollisionManager.h"
#include "../Monster/BaseMonster.h"
#include "Animation/Skeleton.h"

// Sets default values for this component's properties
UHitCollisionManager::UHitCollisionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHitCollisionManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHitCollisionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UHitCollisionManager::InitHitBox(UDataTable* data,USkeletalMeshComponent* mesh) {

}
bool UHitCollisionManager::ReceiveDamage(const FHitResult& hit, const FName socketName,float dmg, int32& OutDamage, bool& weak) {
	return false;
}

void UHitCollisionManager::SearchChild(USkeletalMeshComponent* mesh, FName CurrentBone, TArray<FName>& Part, TArray<FName>& Bones) {
	USkeleton* Skeleton = mesh->SkeletalMesh->Skeleton;

	if (Skeleton == nullptr) return;

	int currentIndex = mesh->GetBoneIndex(CurrentBone);

	if (!Bones.Contains(CurrentBone)) {
		Part.Add(CurrentBone);
		Bones.Add(CurrentBone);
	}

	TArray <int32> Children;
	Skeleton->GetChildBones(currentIndex, Children);

	if (Children.Num() == 0) return;

	for (auto child : Children) {
		SearchChild(mesh, mesh->GetBoneName(child), Part, Bones);
	}
}
TMap<EMonsterPartsType, UHitCollisionPart*> UHitCollisionManager::GetHitBox() {
	return HitBox;
}

UHitCollisionPart::UHitCollisionPart() {
	CanDestroy = false;
	IsDestroy = false;
	IsWeak = false;
	TotalDamage = 0.0f;
	DestroyDamage = 0.0f;
	Defence = 0.0f;
}

void UHitCollisionPart::SetUpData(const FMonsterParts data,TArray<FName> PartBone) {
	Part = data.Part;
	CanDestroy = data.CanDestory;
	IsDestroy = false;
	IsWeak = data.IsWeak;
	TotalDamage = 0.0f;
	DestroyDamage = data.DestroyDamage;
	Defence = data.Defence;

	for (auto bone : PartBone) {
		Bones.Add(bone);
	}


}

bool UHitCollisionPart::CheckGetDamageThisPart(const FHitResult& hit, const FName bone,float dmg, int32& OutDamage, bool& broken, EMonsterPartsType& part, bool& weak) {
	if (Bones.Num()>0&&Bones.Contains(bone)) {

		int32 AcculateDamage = FMath::RoundToInt(((100-Defence) / 100)*dmg);
		TotalDamage += AcculateDamage;
		OutDamage = AcculateDamage;
		weak = IsWeak;

		if (CanDestroy &&
			!IsDestroy &&
			(TotalDamage >= DestroyDamage)) {
			CanDestroy = false;
			IsDestroy = true;
			Defence /= 2;
			broken = true;
			Cast<ABaseMonster>(hit.Actor)->SetBrokenState(part);
		}

		broken = false;

		return true;
	}

	return false;
}
TArray<FName> UHitCollisionPart::GetBones() {
	return Bones;
}