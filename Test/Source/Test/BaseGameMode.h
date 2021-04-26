// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Test.h"
#include "GameFramework/GameMode.h"
#include "Assistant/BaseAssistant.h"
#include "BaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()

	ABaseGameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class ABasePlayerController* PlayerControl;

};
