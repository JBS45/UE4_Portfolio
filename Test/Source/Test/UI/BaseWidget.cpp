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
#include "DamageText.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"


void UBaseWidget::NativeConstruct() {
	Super::NativeConstruct();
	PlayerStatusUI = Cast<UUserWidget>(GetWidgetFromName("BaseBar"));
	CommandBoxUI = Cast<UVerticalBox>(GetWidgetFromName("CommandBox"));
	HPBar = Cast<UProgressBar>(PlayerStatusUI->GetWidgetFromName("HpBar"));
	StaminaBar = Cast<UProgressBar>(PlayerStatusUI->GetWidgetFromName("StaminaBar"));
	TargetMark = Cast<UTargetUI>(GetWidgetFromName("TargetMarkUI"));
	auto Main = Cast<UCanvasPanel>(GetRootWidget());

	if (Main != nullptr) {
		WidgetTree->RootWidget = Main;
	}
	for (auto command : CommandBoxUI->GetAllChildren()) {
		CommandBars.Add(Cast<UCommandWidget>(command));
	}
	TargetMark->SetVisibility(ESlateVisibility::Hidden);

	for (int i = 0; i < PoolLimit; ++i) {
		auto Damage = WidgetTree->ConstructWidget<UDamageText>(DamageText);
		Damage->Init();
		Damage->SetRenderScale(FVector2D(2, 2));
		Main->AddChild(Damage);
		DamageTextPool.Add(Damage);
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
void UBaseWidget::UseDamageText(class ABasePlayerController* control, FVector worldlocation, int32 damage, bool IsCritical) {
	for (auto element : DamageTextPool) {
		if (element->GetIsEnable()) {
			element->UseDamagetText(control, worldlocation, damage, IsCritical);
			break;
		}
	}
}
void UBaseWidget::NotifyStatusData(const FBaseCharacterStatus CharacterStatus) {
	float HPRate = (float)CharacterStatus.CurrentHp / (float)CharacterStatus.MaxHp;
	float StaminaRate = (float) CharacterStatus.CurrentStamina / (float)CharacterStatus.MaxStamina;
	HPBar->SetPercent(HPRate);
	StaminaBar->SetPercent(StaminaRate);
}
void UBaseWidget::NotifyChainData(const TArray<FChainAction> chain) {
	TArray<FChainAction> VisibleChain;

	for (auto element : chain) {
		if (element.AttackType != ECommandName::E_LEFTSTEP
			&& element.AttackType != ECommandName::E_RIGHTSTEP) {
			VisibleChain.Add(element);
		}
	}

	for (auto bar : CommandBars) {
		bar->HideWidget();
	}
	for (int i = 0; i < VisibleChain.Num(); ++i) {
		CommandBars[i]->SetCommand(VisibleChain[i].AttackName, VisibleChain[i].MoveCommand, VisibleChain[i].ActionCommand);
	}
}