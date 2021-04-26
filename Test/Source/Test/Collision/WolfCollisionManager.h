// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitCollisionManager.h"
#include "WolfCollisionManager.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UWolfCollisionManager : public UHitCollisionManager
{
	GENERATED_BODY()
	
public:
	UWolfCollisionManager();
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CreateHitBox(FString socketName) override;
	virtual void SetUpAttachSocket(class USkeletalMeshComponent* mesh) override;
	virtual void SetUpHitBoxInfo(FVector size, FVector pos, FString socketName) override;
	virtual void SetUpCollisionType(FName name, ECollisionChannel channel) override;

protected:
	TMap<FString, FHitBoxInfo> HitBoxInfo;
	TMap<FString, UBoxComponent*> AllHitBox;
	TArray<FString> HitBoxKey;
};
