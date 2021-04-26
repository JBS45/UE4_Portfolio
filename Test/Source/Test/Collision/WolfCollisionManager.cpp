// Fill out your copyright notice in the Description page of Project Settings.


#include "WolfCollisionManager.h"

UWolfCollisionManager::UWolfCollisionManager() {
	CreateHitBox("Head");
	SetUpHitBoxInfo(FVector(56, 32, 32), FVector::ZeroVector, "Head");

	CreateHitBox("Body");
	SetUpHitBoxInfo(FVector(80, 48, 48), FVector(20, -5, 0), "Body");


	CreateHitBox("LeftArm");
	SetUpHitBoxInfo(FVector(48,16, 16), FVector(24, 0, 0), "LeftArm");


	CreateHitBox("RightArm");
	SetUpHitBoxInfo(FVector(48, 16, 16), FVector(24, 0, 0), "RightArm");

	CreateHitBox("LeftLeg");
	SetUpHitBoxInfo(FVector(64, 24, 24), FVector(-20, 0, 0), "LeftLeg");


	CreateHitBox("RightLeg");
	SetUpHitBoxInfo(FVector(64, 24, 24), FVector(-20, 0, 0), "RightLeg");
}

void UWolfCollisionManager::BeginPlay()
{
	Super::BeginPlay();
}

void UWolfCollisionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWolfCollisionManager::CreateHitBox(FString socketName)
{
	UBoxComponent* TempBox = CreateDefaultSubobject<UBoxComponent>(*socketName);
	HitBoxKey.Add(socketName);
	AllHitBox.Add(socketName, TempBox);
}

void UWolfCollisionManager::SetUpAttachSocket(USkeletalMeshComponent * mesh)
{
	for (auto key : HitBoxKey) {
		AllHitBox[key]->SetupAttachment(mesh, FName(*key));
		AllHitBox[key]->SetBoxExtent(HitBoxInfo[key].BoxSize);
		AllHitBox[key]->SetRelativeTransform(FTransform(FQuat::Identity, HitBoxInfo[key].BoxPos, FVector::OneVector));
		AllHitBox[key]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		
	}
}

void UWolfCollisionManager::SetUpHitBoxInfo(FVector size, FVector pos, FString socketName)
{
	FHitBoxInfo Temp;
	Temp.BoxSize = size;
	Temp.BoxPos = pos;

	HitBoxInfo.Add(socketName, Temp);
}
void UWolfCollisionManager::SetUpCollisionType(FName name, ECollisionChannel channel) {
	for (auto key : HitBoxKey) {
		AllHitBox[key]->SetCollisionObjectType(channel);
		AllHitBox[key]->SetCollisionProfileName(name);
	}

}
