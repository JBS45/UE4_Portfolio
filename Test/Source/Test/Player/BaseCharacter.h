// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../WeaponAnimTable.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "BaseCharAnimInstance.h"
#include "../Components/CharacterStatusManager.h"
#include "GameFramework/Character.h"
#include "../Collision/HumanCollisionManager.h"
#include "../Components/DamageInterface.h"
#include "BaseCharacter.generated.h"

class ABaseWeapon;
class UDetectComponent;
class UPostProcessComponent;

UCLASS()
class TEST_API ABaseCharacter : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ECharacterState CurrentState;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, Category = "Status")
		class ABaseWeapon* LeftHand;
	UPROPERTY(EditAnywhere, Category = "Status")
		class ABaseWeapon* RightHand;

	UPROPERTY(EditAnywhere, Category = "Movvement", meta = (AllowPrivateAccess = "true"))
		float BasicSpeed = 800.0f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float RunSpeed = 1600.0f;


	class USkeletalMesh* TempMesh;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		UHumanCollisionManager* CollisionManager;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		UCharacterStatusManager* StatusManager;
	UPROPERTY(VisibleAnywhere, Category = "Animation")
		UBaseCharAnimInstance* AnimInst;
	UPROPERTY(VisibleAnywhere, Category = "Detect")
		class UDetectComponent* Detect;
	UPROPERTY(VisibleAnywhere, Category = "PostProcess")
		class UPostProcessComponent* PostProcessMat;

	bool IsAlive = true;

	float CheckBattleTime = 0.0f;
	float DetectRange = 2000.0f;
	bool IsBlock = false;

	AActor* BlockTarget;

public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		UCameraComponent* Camera;

	float MinCameraDistance = 150;
	float MaxCameraDistance = 600;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		float CurrentSpringArmLength;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		float WheelSpeed;

	FVector BaseCameraVector;
	FRotator BaseCameraRotator;

	FPlayerAttackDel AttackDel;
	FTimerHandle CheckTimer;
public:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void CameraZoom(float NewAxisValue);
	void MoveCameraMaxDist();
	void CameraLockUp(float NewAxisValue);
	void CameraTurn(float NewAxisValue);

	void Sprint(bool sprint);
	void Evade();


public:
	FORCEINLINE UBaseCharAnimInstance* GetAnimInst();
	FORCEINLINE EWeaponType GetWeaponState();
	FORCEINLINE UCharacterStatusManager* GetCharacterStatus();
	FORCEINLINE bool GetIsAlive() const;
	FORCEINLINE class ABaseWeapon* GetLeftHand();
	FORCEINLINE class ABaseWeapon* GetRightHand();

	void SetIsSprint(bool value);
	void ChangeWeapon(EWeaponType type);
	void CharacterChangeState(ECharacterState state);

	TArray<APawn*> GetTargetMonster();
	void CameraLockOn(class ABaseMonster* target);
	float GetDetectRange() {return DetectRange;}
	void SetInitWeapon();
	void DrawWeapon();
	void PutUpWeapon();
	FORCEINLINE void WeaponLeftOverlapOn();
	FORCEINLINE void WeaponLeftOverlapOff();
	FORCEINLINE void WeaponRightOverlapOn();
	FORCEINLINE void WeaponRightOverlapOff();

	bool CaculateCritical();
	void RadialBlurOn();
	void RadialBlurOff();

	virtual void ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0);
	virtual void TakeDamageFunc(bool& OutIsWeak, int32& OutFinalDamage, AActor* Causer, const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0);
private:
	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	FVector BlockCheck(AActor* Actor);
	void Blocking(float delta);
};
