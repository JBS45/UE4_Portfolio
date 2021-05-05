// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "HitCollisionManager.h"
#include "DragonCollisionManager.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UDragonCollisionManager : public UHitCollisionManager
{
	GENERATED_BODY()
public:
	UDragonCollisionManager();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//new
	virtual void InitHitBox(UDataTable* data, USkeletalMeshComponent* mesh) override;
	virtual bool ReceiveDamage(const FHitResult& hit, const FName socketName, float dmg,int32& outDmage, bool& weak) override;
};
