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
	FVoidDelegateOne OnChangeCharacterState;
	// Called every frame

	void StatusInit();
	void EquipItem(const FEquipmentStatus item);
	void ReGen(const float deltaTime);
	bool UseStamina();

	float GetHpRate() { return CharacterStatus.CurrentHp / CharacterStatus.MaxHp; }
	float GetStaminaRate() { return CharacterStatus.CurrentStamina / CharacterStatus.MaxStamina; }

	void SetIsSprint(bool value) { IsSprint = value; }
private:
	void CreateCharState();
	void UpdateStatus();
};
