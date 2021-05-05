// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "GameFramework/Character.h"
#include "../Components/DetectComponent.h"
#include "../Components/DamageInterface.h"
#include "BaseMonster.generated.h"

class UHitCollisionManager;
class UMonsterStatusManager;
class UMonsterAnimInstance;
class AMonsterAIController;
class AMonsterArea;
class ABaseCharacter;

UCLASS()
class TEST_API ABaseMonster : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	virtual bool MonsterInit(FMonsterStatus status) PURE_VIRTUAL(ABaseMonster::MonsterInit, return false;);
	virtual bool TakeDamageFromPlayer(const FHitResult& hit, const FName socketName, float dmg, int32& outDmage, bool& weak)  PURE_VIRTUAL(ABaseMonster::TakeDamage, return false;);
	virtual void ChangeMonsterState(EMonsterState state) PURE_VIRTUAL(ABaseMonster::ChangeMonsterState);
	virtual void Dead() PURE_VIRTUAL(ABaseMonster::Dead);
	virtual void PartBroken(EMonsterPartsType brokenPart) PURE_VIRTUAL(ABaseMonster::PartBroken);
	virtual void Roar() PURE_VIRTUAL(ABaseMonster::Roar);

public:
	virtual void SetBrokenState(EMonsterPartsType brokenPart);
	virtual void ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0);
	virtual void TakeDamageFunc(bool& OutIsWeak, int32& OutFinalDamage, AActor* Causer, const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0);
	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FORCEINLINE float GetAggroChangeTerm() const;
	FORCEINLINE float GetAttackRange() const;
	FORCEINLINE void SetActiveArea(class AMonsterArea* area);
	FORCEINLINE AMonsterArea* GetActiveArea();
	FORCEINLINE UHitCollisionManager* GetCollisionManager();
	FORCEINLINE FString GetMonsterName() const ;
	FORCEINLINE uint8 GetMonsterID() const;
	FORCEINLINE TArray<APawn*> GetTargetList();
	FORCEINLINE EMonsterState GetMonsterState();
	FORCEINLINE int32 GetBrokenState() const;
	FORCEINLINE ABaseCharacter* GetTarget();
	FORCEINLINE AMonsterAIController* GetMonsterController();
	FORCEINLINE UMonsterAnimInstance* GetAnimInst();
	FORCEINLINE float GetRoarRange();

	FORCEINLINE UAnimMontage* GetMontage(FString name) const;

	void PartOverlapOn(EMonsterPartsType Parts);
	void PartOverlapOff();
	void SetMeleeDamage(float damageRate, float force, EDamageType type, EMonsterPartsType EnablePart);
	TArray<UCapsuleComponent*> GetDamageBox(EMonsterPartsType Parts);
	void HitCheck(FName Bone);

protected:
	void GetDamageCapsules();
	void SetDamageBox(TArray<class UCapsuleComponent*> arr);
	void SetUpAnim(const FString DataRef);
	void ResetDamagedPlayer();

protected:

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float BasicSpeed = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float RunSpeed = 200.0f;
	UPROPERTY(VisibleAnywhere, Category = "CollisionManager", meta = (AllowPrivateAccess = "true"))
		class UHitCollisionManager* HitBox;
		TMultiMap<EMonsterPartsType,UCapsuleComponent*> DamageBox;
		TMap<FString, UAnimMontage*> AnimationMap;

	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		class UMonsterStatusManager* Status;
	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FString MonsterName;
	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		uint8 MonsterID;
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float AttackRange;
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float AggroChangeTerm;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		EMonsterState CurrentState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EMonsterBrokenParts"))
		int32 BrokenState;
	UPROPERTY(EditAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		float RoarRange = 2000.0f;
	UPROPERTY(VisibleAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UMonsterAnimInstance* AnimInst;
	UPROPERTY(VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
		AMonsterAIController* MonsterController;

	UPROPERTY(VisibleAnywhere, Category = "Area", meta = (AllowPrivateAccess = "true"))
		AMonsterArea* ActiveArea;
	UPROPERTY(VisibleAnywhere, Category = "Area", meta = (AllowPrivateAccess = "true"))
		ABaseCharacter* Target;

	UPROPERTY(VisibleAnywhere, Category = "Detect", meta = (AllowPrivateAccess = "true"))
		UDetectComponent* _Detect;
	UPROPERTY(EditAnywhere, Category = "Detect", meta = (AllowPrivateAccess = "true"))
		float DetectRange = 1500.0f;

protected:
	bool IsAttack = false;
	int MaxPatrolNode;
	int CurrentPatrolNode;
	TArray<ABaseCharacter*> DamagedCharacter;

	EDamageType AttackDamageType = EDamageType::E_NORMAL;
	float DamageRate = 1.0f;
	float KnockBackForce = 0.0f;
	EMonsterPartsType AttackEnablePart = EMonsterPartsType::E_HEAD;

};
