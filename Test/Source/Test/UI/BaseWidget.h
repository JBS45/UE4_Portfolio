// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "Blueprint/UserWidget.h"
#include "../BaseStatus.h"
#include "../BaseEnum.h"
#include "BaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	void BindCharacterStatus(class UCharacterStatusManager* status);
	void BindInputCommand(class UInputBufferManager* inputbuffer);
	void UpdateStatus();
	void UpdateCommand();
	void SetCameraLockOn(bool IsOn);
	void TraceTarget(class ABasePlayerController* control, class ABaseMonster* target);
private:
	class UCharacterStatusManager* CurrentCharacterStatus;
	class UInputBufferManager* InputBuffer;

	UPROPERTY()
		class UUserWidget* PlayerStatusUI;
	UPROPERTY()
		class UVerticalBox* CommandBoxUI;
	UPROPERTY()
		TArray<class UCommandWidget*> CommandBars;
	UPROPERTY()
		class UProgressBar* HPBar;
	UPROPERTY()
		class UProgressBar* StaminaBar;
	UPROPERTY()
		class UTargetUI* TargetMark;
};
