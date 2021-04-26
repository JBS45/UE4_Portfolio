// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanCollisionManager.h"
#include "Components/SkeletalMeshComponent.h"
UHumanCollisionManager::UHumanCollisionManager() {

	CreateHitBox("Head");
	SetUpHitBoxInfo(FVector(16, 10, 10), FVector::ZeroVector, "Head");

	CreateHitBox("Body");
	SetUpHitBoxInfo(FVector(32, 16, 16), FVector(20,-5,0), "Body");

	CreateHitBox("LeftUpperArm");
	SetUpHitBoxInfo(FVector(24, 8, 8), FVector(16, 0, 0), "LeftUpperArm");

	CreateHitBox("LeftArm");
	SetUpHitBoxInfo(FVector(24, 8, 8), FVector(24, 0, 0), "LeftArm");

	CreateHitBox("RightUpperArm");
	SetUpHitBoxInfo(FVector(24, 8, 8), FVector(-16, 0, 0), "RightUpperArm");

	CreateHitBox("RightArm");
	SetUpHitBoxInfo(FVector(24, 8, 8), FVector(-24, 0, 0), "RightArm");

	CreateHitBox("LeftUpperLeg");
	SetUpHitBoxInfo(FVector(32, 12, 12), FVector(-20, 0, 0), "LeftUpperLeg");

	CreateHitBox("LeftLeg");
	SetUpHitBoxInfo(FVector(32, 12, 12), FVector(-20, 0, 0), "LeftLeg");

	CreateHitBox("RightUpperLeg");
	SetUpHitBoxInfo(FVector(32, 12, 12), FVector(20, 0, 0), "RightUpperLeg");

	CreateHitBox("RightLeg");
	SetUpHitBoxInfo(FVector(32, 12, 12), FVector(20, 0, 0), "RightLeg");
}

void UHumanCollisionManager::BeginPlay()
{
	Super::BeginPlay();
}

void UHumanCollisionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UHumanCollisionManager::CreateHitBox(FString socketName) {
	UBoxComponent* TempBox = CreateDefaultSubobject<UBoxComponent>(*socketName);
	TempBox->SetHiddenInGame(false);
	HitBoxKey.Add(socketName);
	AllHitBox.Add(socketName, TempBox);
}
void UHumanCollisionManager::SetUpAttachSocket(class USkeletalMeshComponent* mesh) {

	for (auto key : HitBoxKey) {
		AllHitBox[key]->SetupAttachment(mesh,FName(*key));
		AllHitBox[key]->SetBoxExtent(HitBoxInfo[key].BoxSize);
		AllHitBox[key]->SetRelativeTransform(FTransform(FQuat::Identity, HitBoxInfo[key].BoxPos, FVector::OneVector));
		AllHitBox[key]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	}

}
void UHumanCollisionManager::SetUpHitBoxInfo(FVector size, FVector pos, FString socketName) {
	FHitBoxInfo Temp;
	Temp.BoxSize = size;
	Temp.BoxPos = pos;

	HitBoxInfo.Add(socketName, Temp);
}
void UHumanCollisionManager::SetUpCollisionType(FName name, ECollisionChannel channel) {
	for (auto key : HitBoxKey){
		AllHitBox[key]->SetCollisionObjectType(channel);
		AllHitBox[key]->SetCollisionProfileName(name);
	}

}