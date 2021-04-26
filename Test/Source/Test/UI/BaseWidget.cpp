// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"
#include "CommandWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../Components/CharacterStatusManager.h"
#include "../Input/InputBufferManager.h"
#include "TargetUI.h"
#include "../Monster/BaseMonster.h"
#include "../Player/BasePlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UBaseWidget::NativeConstruct() {
	Super::NativeConstruct();
	PlayerStatusUI = Cast<UUserWidget>(GetWidgetFromName("BaseBar"));
	CommandBoxUI = Cast<UVerticalBox>(GetWidgetFromName("CommandBox"));
	HPBar = Cast<UProgressBar>(PlayerStatusUI->GetWidgetFromName("HpBar"));
	StaminaBar = Cast<UProgressBar>(PlayerStatusUI->GetWidgetFromName("StaminaBar"));
	TargetMark = Cast<UTargetUI>(GetWidgetFromName("TargetMarkUI"));

	for (auto command : CommandBoxUI->GetAllChildren()) {
		CommandBars.Add(Cast<UCommandWidget>(command));
	}
	TargetMark->SetVisibility(ESlateVisibility::Hidden);
}
void UBaseWidget::BindCharacterStatus(UCharacterStatusManager * status)
{
	if (status != nullptr) {
		CurrentCharacterStatus = status;
	}
	CurrentCharacterStatus->OnStatusUpdate.BindUObject(this, &UBaseWidget::UpdateStatus);
}

void UBaseWidget::BindInputCommand(UInputBufferManager * inputbuffer)
{
	if (inputbuffer != nullptr) {
		InputBuffer = inputbuffer;
	}
	InputBuffer->CommandUpdateDelegate.BindUObject(this, &UBaseWidget::UpdateCommand);
}

void UBaseWidget::UpdateStatus()
{
	if (IsValid(CurrentCharacterStatus)) {
		HPBar->SetPercent(CurrentCharacterStatus->GetHpRate());
		StaminaBar->SetPercent(CurrentCharacterStatus->GetStaminaRate());
	}
}

void UBaseWidget::UpdateCommand() {
	for (auto slot : CommandBars) {
		slot->HideWidget();
	}
	if (IsValid(InputBuffer)) {
		auto EnableAction = InputBuffer->GetEnableAction();

		for (int i = 0; i < EnableAction.Num(); ++i) {
			CommandBars[i]->SetCommand(EnableAction[i], InputBuffer->GetCommandFromName(EnableAction[i]));
		}
	}
}

void UBaseWidget::SetCameraLockOn(bool IsOn) {
	if (IsOn) {
		TargetMark->SetVisibility(ESlateVisibility::Visible);
		TargetMark->PlayAnim();
	}
	else {
		TargetMark->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UBaseWidget::TraceTarget(class ABasePlayerController* control, class ABaseMonster* target) {
	FVector2D ScreenPos;
	if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(control, target->GetActorLocation(), ScreenPos, false)) {
		TargetMark->SetRenderTranslation(ScreenPos);
	}
}