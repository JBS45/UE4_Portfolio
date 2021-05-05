// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "Blueprint/UserWidget.h"
#include "DamageText.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UDamageText : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	void PlayAnim();
	void FinshEvent();
	void Init();
	void UseDamagetText(class ABasePlayerController* control,FVector worldlocation, float damage, bool IsCritical);
	bool GetIsEnable() { return IsEnable; }
private:
	UPROPERTY()
		class UTextBlock* TextBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool IsEnable = true;
private:
	UPROPERTY(meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"))
		class UWidgetAnimation* Floating;
};
