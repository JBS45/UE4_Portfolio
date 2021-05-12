// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h"
#include "HitCollisionManager.generated.h"

class USkeletalMeshComponent;

UCLASS()
class TEST_API UHitCollisionManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitCollisionManager();

protected:
	TMap<FName, UHitCollisionPart*> HitBox;
	TMap<EMonsterPartsType, UHitCollisionPart*> HitBoxLabel;
	TMap<FName, EMonsterPartsType> PartLabel;
	TMap<EMonsterPartsType, USkeletalBodySetup*> DamageBox;
	UDataTable* DataTable;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SearchChild(USkeletalMeshComponent* mesh, FName CurrentBone, EMonsterPartsType type);
public:		
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
	//virtual Hit Check
	virtual void InitHitBox(UDataTable* data,USkeletalMeshComponent* mesh);
	virtual bool ReceiveDamage(const FHitResult& hit, const FName socketName, float dmg, int32& OutDamage, bool& weak);
	virtual void BrokenPart(EMonsterPartsType type);
	FORCEINLINE USkeletalBodySetup* GetBoneNames(const EMonsterPartsType part) const;
};
UCLASS()

class TEST_API UHitCollisionPart : public UObject {
	GENERATED_BODY()
public:
	UHitCollisionPart();
private:
	EMonsterPartsType Part;
	bool CanDestroy;
	bool IsDestroy;
	bool IsWeak;
	float TotalDamage;
	float DestroyDamage;
	float Defence;

public:
	void SetUpData(const FMonsterParts data);
	void SetBroken();
	bool CheckGetDamageThisPart(float dmg, int32& OutDamage, bool& weak);
	EMonsterPartsType GetPart();
};