// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "HitCollisionManager.h"
#include "HumanCollisionManager.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UHumanCollisionManager : public UHitCollisionManager
{
	GENERATED_BODY()
public:
	UHumanCollisionManager();
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CreateHitBox(FString socketName) override;
	virtual void SetUpAttachSocket(class USkeletalMeshComponent* mesh) override;
	virtual void SetUpHitBoxInfo(FVector size, FVector pos, FString socketName) override;
	virtual void SetUpCollisionType(FName name, ECollisionChannel channel) override;
};
