// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "../Components/DetectComponent.h"
#include "GameFramework/Character.h"
#include "BaseAssistant.generated.h"

UCLASS()
class TEST_API ABaseAssistant : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAssistant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Character")
		class ABaseCharacter* _Master;

	//Todo : Make UBaseStatusManager And Change this
	UPROPERTY(VisibleAnywhere, Category = "Status")
		class ABaseWeapon* Weapon;
	class UStaticMesh* WeaponMesh;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		class UCharacterStatusManager* _StatusManager;

	UPROPERTY(VisibleAnywhere, Category = "Animation")
		class UAssistantInstance* _AnimInst;

	UPROPERTY(VisibleAnywhere, Category = "Detect")
		UDetectComponent* _Detect;

	class ABaseMonster* Target;

	//Todo : Make Simple AI Controller

	//Todo : Bind IsBattle of _Master;
	bool IsBattle = false;
	bool CanAction = true;
	bool IsAlive = true;
	const float ActionCoolTime = 5.0f;
	float ActionTimer = 0.0;
	float DetectRange = 2000.0f;
	float FollowRange = 300.0f;
	float AttackRange = 200.0f;

	TMap<FString,FAssistSkill> Skills;
public:
	void SetMaster(class ABaseCharacter* master) { _Master = master; }
	class ABaseCharacter* GetMaster() {return _Master;}
	float PlayerDistance();
	float GetFollowRange() { return FollowRange; }
	bool BaseAttack();
	FOnAttackEndDelegate OnAttackEnd;
	void OnAttackMontageBegin();
	void OnAttackMontageEnd();

	void SetIsBattle(bool value);
	bool GetIsBattle() { return IsBattle; }
	bool GetCanAction() { return CanAction; }
	bool GetIsAlive() { return IsAlive; }
	class ABaseMonster* GetTarget() { return Target; }
	TArray<APawn*> GetTargetList() { return _Detect->GetTargets(); }
	void SetTarget(class ABaseMonster* target) { Target = target; }
	float GetDetectRange() { return DetectRange; }
	float GetAttackRange() { return AttackRange; }
private:
	void SetUpSkill();
	void SkillUpdate(float Deltaseconds);
	bool UseSkill(FString skillName);
	void ActionCoolTimer(float Deltaseconds);
};
