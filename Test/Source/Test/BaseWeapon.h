// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Test.h"
#include "BaseEnum.h"
#include "BaseStatus.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UParticleSystemComponent;

UCLASS()
class TEST_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

	UPROPERTY(VisibleAnywhere,  Category = "Trail", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* TrailParticle;
	UPROPERTY(VisibleAnywhere,  Category = "Trail", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UParticleSystem> TrailTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	FBaseStatus Status;

	TArray<class ABaseMonster*> DamagedMonster;

	class ACharacter* WeaponOwner;
	float DamageRate;
public:
	void SetUpWeapon(EWeaponType type, USkeletalMesh* mesh, ACharacter* owner);
	void SetEnableWeapon(bool value);
	bool HitCheck(class AActor* Actor);
	void ResetDamagedMonster();
	void SetOverlapEvent(bool IsOn);
	void SetDamageRate(float value);

	void TrailOn(ETrailWidthMode type,float value);
	void TrailOff();
private:
	FVector CurrentBoxSize;
	FVector CurrentBoxLocation;
};
