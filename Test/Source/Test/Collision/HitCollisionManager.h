// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h"
#include "HitCollisionManager.generated.h"

class USkeletalMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), abstract )
class TEST_API UHitCollisionManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitCollisionManager();

protected:
	TMap<EMonsterPartsType, UHitCollisionPart*> HitBox;
	TArray<FString> HitBoxKey;
	UDataTable* DataTable;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:		
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
	//virtual Hit Check
	virtual void InitHitBox(UDataTable* data,USkeletalMeshComponent* mesh);
	virtual bool ReceiveDamage(const FHitResult& hit, const FName socketName, float dmg, int32& OutDamage, bool& weak);
	FORCEINLINE TMap<EMonsterPartsType, UHitCollisionPart*> GetHitBox();
protected:
	void SearchChild(USkeletalMeshComponent* mesh, FName CurrentBone, TArray<FName>& Part, TArray<FName>& Bones);
};
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))

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
	TArray<FName> Bones;

public:
	void SetUpData(const FMonsterParts data, TArray<FName> PartBone);
	bool CheckGetDamageThisPart(const FHitResult& hit,const FName socketName, float dmg, int32& OutDamage, bool& broken, EMonsterPartsType& part, bool& weak);
	FORCEINLINE TArray<FName> GetBones();
};