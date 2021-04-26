// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h"
#include "HitCollisionManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), abstract )
class TEST_API UHitCollisionManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitCollisionManager();

protected:
	/*
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Head;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Body;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* LeftUpperArm;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* LeftArm;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RightUpperArm;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RightArm;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* LeftUpperLeg;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* LeftLeg;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RightUpperLeg;
	UPROPERTY(VisibleAnywhere,  Category = "Collison", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RightLeg;
		*/
	TMap<FString,FHitBoxInfo> HitBoxInfo;
	TMap<FString,UBoxComponent*> AllHitBox;
	TArray<FString> HitBoxKey;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CreateHitBox(FString socketName) PURE_VIRTUAL(UHitCollisionManager::CreateHitBox);
	virtual void SetUpAttachSocket(class USkeletalMeshComponent* mesh) PURE_VIRTUAL(UHitCollisionManager::SetUpAttachSocket);
	virtual void SetUpHitBoxInfo(FVector size, FVector pos, FString socketName) PURE_VIRTUAL(UHitCollisionManager::SetUpHitBoxInfo);
	virtual void SetUpCollisionType(FName name, ECollisionChannel channel) PURE_VIRTUAL(UHitCollisionManager::SetUpCollisionType);
	
	//virtual Hit Check

};
