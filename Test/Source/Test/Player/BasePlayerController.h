// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "BaseCharacter.h"
#include "../UI/BaseWidget.h"
#include "../Assistant/BaseAssistant.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UInputBufferManager;
class UComandTableManager;

UCLASS()
class TEST_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();
	
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Character")
		ABaseCharacter* CurrentCharaceter;
	UPROPERTY(VisibleAnywhere, Category = "State")
		EWeaponType CurrentWeapon;
	UPROPERTY(VisibleAnywhere, Category = "State")
		ECharacterState CharacterState;
	UPROPERTY(VisibleAnywhere, Category = "Input")
		UInputBufferManager* InputBuffer;
	UPROPERTY(VisibleAnywhere, Category = "Input")
		UComandTableManager* CommandTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UBaseWidget> HUDWidgetClass;
	UPROPERTY()
		class UBaseWidget* PlayerHUD;

	class ABaseMonster* Target;
	bool IsLockOn = false;

	bool CanMove = true;
	bool IsSprint = false;
	bool IsSpecial = false;


public:
	UAnimMontage* TestAnim;
	FCharacterState ChangeStateDel;
	FCharacterSprint SprintDel;
	FWeaponType ChangeWeaponDel;

public:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void CameraLookUp(float Value);
	void CameraTurn(float Value);
	//void CameraZoom(float Value);

public:
	void PressShift();
	void ReleaseShift();
	void MouseLeftClick();
	void MouseRightClick();
	void SpaceKey();
	void ToggleLockOn();
	void ChangeLockOnTarget();

	void CameraLockOn(float deltaTime);
public:
	void ChangeCharacterState(ECharacterState state);
	void ChangeWeapon(EWeaponType type);
	UInputBufferManager* GetInputBuffer();
	class ABaseMonster* GetTarget();

private:
	void AttachWidgetToViewport(TSubclassOf<class UBaseWidget> widget);
	FORCEINLINE void OnDead();
	FORCEINLINE void NoStatmina();

	void PressW();
	void ReleaseW();
	void PressS();
	void ReleaseS();
	void PressD();
	void ReleaseD();
	void PressA();
	void ReleaseA();

};
