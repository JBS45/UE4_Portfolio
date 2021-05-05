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
	void UpdateStatus();
	void UpdateCommand(TArray<FChainAction> chainAction);
	void SetCameraLockOn(bool IsOn);
	void TraceTarget(class ABasePlayerController* control, class ABaseMonster* target);
	void UseDamageText(class ABasePlayerController* control, FVector worldlocation, int32 damage, bool IsCritical);
private:
	class UCharacterStatusManager* CurrentCharacterStatus;
	class UCommandTableManager* CommandTable;

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
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UDamageText> DamageText;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<class UDamageText*> DamageTextPool;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 PoolLimit = 50;
};
