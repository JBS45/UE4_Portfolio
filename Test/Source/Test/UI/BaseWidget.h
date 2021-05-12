// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "Blueprint/UserWidget.h"
#include "../BaseStatus.h"
#include "../BaseEnum.h"
#include "../Components/MyInterface.h"
#include "BaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UBaseWidget : public UUserWidget, public IStatusInterface, public IChainInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	void SetCameraLockOn(bool IsOn);
	void TraceTarget(class ABasePlayerController* control, class ABaseMonster* target);
	void UseDamageText(class ABasePlayerController* control, FVector worldlocation, int32 damage, bool IsCritical);
	virtual void NotifyStatusData(const FBaseCharacterStatus CharacterStatus) override;
	virtual void NotifyChainData(const TArray<FChainAction> chain) override;
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
