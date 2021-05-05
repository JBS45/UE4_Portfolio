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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
	//new
	virtual void InitHitBox(UDataTable* data, USkeletalMeshComponent* mesh) override;
	virtual bool ReceiveDamage(const FHitResult& hit, const FName socketName, float dmg, int32& outDmage,bool& IsWeak) override;

public:
	void SetUpCharacterStatusManager(class ABaseCharacter* character);
private:
	class ABaseCharacter* Character;
	class UCharacterStatusManager* StatusManager;
};
