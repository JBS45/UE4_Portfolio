// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Test.h"
#include "../BaseEnum.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CommandWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UCommandWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCommandWidget(const FObjectInitializer& objectInitializer);
protected:
		
	virtual void NativeConstruct() override;

	TArray<UImage*> CommandImages;
	UTextBlock* CommandName;
	UTextBlock* KeyName;

	TArray<UTexture2D*> ImageArray;
	UTexture2D* PlusImage;
	
public:
	void HideWidget();
	void SetCommand(FString commandName, TArray<EMoveKey> move, TArray<EActionKey> Action);
	
private:
	void LoadImage();
	void SetImage(TArray<EMoveKey> move, TArray<EActionKey> Action);
	UTexture2D* SelectImage(EMoveKey move);
	UTexture2D* SelectImage(TArray<EActionKey> action);
};