// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../WeaponAnimTable.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "BaseCharAnimInstance.h"
#include "../Components/MovementManager.h"
#include "../Components/CharacterStatusManager.h"
#include "GameFramework/Character.h"
#include "../Collision/HumanCollisionManager.h"
#include "BaseCharacter.generated.h"

UCLASS()
class TEST_API ABaseCharacter : public ACharacter
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
		bool IsDrawWeapon;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		EWeaponType _WeaponType;
	UPROPERTY(EditAnywhere, Category = "Status")
		class ABaseWeapon* LeftHand;
	UPROPERTY(EditAnywhere, Category = "Status")
		class ABaseWeapon* RightHand;
	class UStaticMesh* TempMesh;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
		UHumanCollisionManager* _CollisionManager;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		UCharacterStatusManager* _StatusManager;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		UMovementManager* _MovementStatus;
	UPROPERTY(VisibleAnywhere, Category = "Animation")
		UBaseCharAnimInstance* _AnimInst;
	UPROPERTY(VisibleAnywhere, Category = "Detect")
		class UDetectComponent* _Detect;

	bool IsAlive = true;

	float CheckBattleTime = 0.0f;
	float DetectRange = 2000.0f;

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
	bool GetIsDrawWeapon() { return IsDrawWeapon; }
	UMovementManager* GetMovementStatus(){ return _MovementStatus; }
	UBaseCharAnimInstance* GetAnimInst() { return _AnimInst; }
	void SetIsDrawWeapon(bool value) { IsDrawWeapon = value; }
	EWeaponType GetWeaponState() {return _WeaponType;}
	UCharacterStatusManager* GetCharacterStatus() { return _StatusManager; }
	bool GetIsAlive() { return IsAlive; }

	void SetMovement(EMovementState state);
	void SetIsSprint(bool value);
	void ChangeWeapon(EWeaponType type);

	TArray<APawn*> GetTargetMonster();
	void CameraLockOn(class ABaseMonster* target);
	float GetDetectRange() {return DetectRange;}
	void SetInitWeapon();
	void DrawWeapon();
	void PutUpWeapon();
};
