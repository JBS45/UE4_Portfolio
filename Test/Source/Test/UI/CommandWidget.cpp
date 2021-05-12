// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandWidget.h"
#include "../BaseStatus.h"


UCommandWidget::UCommandWidget(const FObjectInitializer& objectInitializer) : Super(objectInitializer) {
	LoadImage();
}
void UCommandWidget::NativeConstruct() {
	Super::NativeConstruct();
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Move1")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Move2")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Plus")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Action")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Key")));
	CommandName = Cast<UTextBlock>(GetWidgetFromName("CommandName"));
	KeyName = Cast<UTextBlock>(GetWidgetFromName("KeyText"));


}

void UCommandWidget :: HideWidget() {
	for (auto image : CommandImages) {
		image->SetVisibility(ESlateVisibility::Hidden);
	}
	CommandName->SetVisibility(ESlateVisibility::Hidden);
	KeyName->SetVisibility(ESlateVisibility::Hidden);

}
void UCommandWidget :: SetCommand(FString commandName, TArray<EMoveKey> move, TArray<EActionKey> Action) {
	if (Action.Num() > 0) {
		CommandName->SetVisibility(ESlateVisibility::Visible);
		CommandName->SetText(FText::FromString(commandName));

		SetImage(move, Action);
	}
}

void UCommandWidget::LoadImage() {
	static ConstructorHelpers::FObjectFinder<UTexture2D> LMBIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/LMB.LMB'"));
	if (LMBIMAGE.Succeeded())
	{
		ImageArray.Add(LMBIMAGE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> RMBIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/RMB.RMB'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(RMBIMAGE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> FRONTIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Up.Up'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(FRONTIMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> BACKIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Down.Down'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(BACKIMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> LEFTIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Left.Left'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(LEFTIMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> RIGHTIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Right.Right'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(RIGHTIMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> SPACEBAR(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/SpaceBar.SpaceBar'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(SPACEBAR.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> PLUS(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Plus.Plus'"));
	if (PLUS.Succeeded())
	{
		PlusImage = PLUS.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> RLMB(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/LMB_RMB.LMB_RMB'"));
	if (PLUS.Succeeded())
	{
		ImageArray.Add(RLMB.Object);
	}
	
}
void UCommandWidget::SetImage(TArray<EMoveKey> move, TArray<EActionKey> Action) {
	if (Action[0] == EActionKey::E_SPECIAL) {
		CommandImages[4]->SetVisibility(ESlateVisibility::Visible);
		KeyName->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		CommandImages[3]->SetVisibility(ESlateVisibility::Visible);
		CommandImages[3]->Brush.SetResourceObject(SelectImage(Action));
	}
	if(move.Num()>0) {

		for (int i = 0; i < move.Num(); ++i) {
			CommandImages[i]->SetVisibility(ESlateVisibility::Visible);
			CommandImages[i]->Brush.SetResourceObject(SelectImage(move[i]));
		}

		CommandImages[2]->SetVisibility(ESlateVisibility::Visible);
		CommandImages[2]->Brush.SetResourceObject(PlusImage);
	}
}
UTexture2D* UCommandWidget::SelectImage(EMoveKey move) {
	UTexture2D* Result = nullptr;

	switch (move) {
		case EMoveKey::E_ALLMOVE:
		case EMoveKey::E_FORWARD:
			Result=ImageArray[2];
			break;
		case EMoveKey::E_BACKWARD:
			Result=ImageArray[3];
			break;
		case EMoveKey::E_LEFT:
			Result=ImageArray[4];
			break;
		case EMoveKey::E_RIGHT:
			Result=ImageArray[5];
			break;
	}
	
	return Result;
}
UTexture2D* UCommandWidget::SelectImage(TArray<EActionKey> action) {
	UTexture2D* Result = nullptr;

	if (action.Num() > 1) {
		Result = ImageArray[7];
	}
	else {
		for (auto element : action) {
			switch (element) {
			case EActionKey::E_LEFTCLICK:
				Result = ImageArray[0];
				break;
			case EActionKey::E_RIGHTCLICK:
				Result = ImageArray[1];
				break;
			case EActionKey::E_EVADE:
				Result = ImageArray[6];
				break;
			}
		}
	}
	return Result;
}