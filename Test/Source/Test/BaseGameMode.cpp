// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Player/BaseCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Player/BasePlayerController.h"

ABaseGameMode::ABaseGameMode() {
	
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
	
}

void ABaseGameMode::PostLogin(APlayerController * NewPlayer)
{
	TESTLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	PlayerControl = Cast<ABasePlayerController>(NewPlayer);
	TESTLOG(Warning, TEXT("PostLogin End"));
}

void ABaseGameMode::BeginPlay() {
	Super::BeginPlay();

	TESTLOG(Warning, TEXT("Game mode begin play"));
}



