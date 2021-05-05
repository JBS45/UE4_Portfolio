// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseStatus.h"
#include "../BaseEnum.h"
#include "Components/ActorComponent.h"
#include "CharacterStatusManager.generated.h"




UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEST_API UCharacterStatusManager : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UCharacterStatusManager();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	const float ReGenBaseTime = 1.0f;

	bool IsAlive;
	bool IsSprint;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		FBaseCharacterStatus CharacterStatus;
public:	

	FVoidDelegate OnStatusUpdate;
	FVoidDelegate OnCharacterDeadDel;
	FVoidDelegate OnStaminaZeroDel;

	// Called every frame

	void StatusInit();
	void EquipItem(const FEquipmentStatus item);
	void ReGen(const float deltaTime);
	bool UseStamina();

	float GetHpRate();
	float GetStaminaRate();
	float GetDamage();
	float GetCritical();

	void SetIsSprint(bool value);
	int32 TakeDamage(float Damage);

private:
	void CreateCharState();
	void UpdateStatus();

	void RegenStamina(float delta);
	void SprintUseStamina(float delta);
};
