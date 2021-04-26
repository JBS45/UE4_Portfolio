// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandWidget.h"


UCommandWidget::UCommandWidget(const FObjectInitializer& objectInitializer) : Super(objectInitializer) {
	LoadImage();
}
void UCommandWidget::NativeConstruct() {
	Super::NativeConstruct();
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Command1")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Plus")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Command2")));
	CommandName = Cast<UTextBlock>(GetWidgetFromName("CommandName"));


}

void UCommandWidget :: HideWidget() {
	for (auto image : CommandImages) {
		image->SetVisibility(ESlateVisibility::Hidden);
	}
	CommandName->SetVisibility(ESlateVisibility::Hidden);

}
void UCommandWidget :: SetCommand(FString commandName, TArray<EInputKey> command) {
	CommandName->SetVisibility(ESlateVisibility::Visible);
	CommandName->SetText(FText::FromString(commandName));

	SetImage(command);
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
	
}
void UCommandWidget::SetImage(TArray<EInputKey> command) {
	if (command.Num() == 1) {
		CommandImages[0]->SetVisibility(ESlateVisibility::Visible);

		
		CommandImages[0]->Brush.SetResourceObject(SelectImage(command[0]));
	}
	else if (command.Num() == 2) {
		for (auto image : CommandImages) {
			image->SetVisibility(ESlateVisibility::Visible);
			CommandImages[0]->Brush.SetResourceObject(SelectImage(command[0]));
			if (CommandImages[1]->Brush.GetResourceObject()==nullptr) {
				CommandImages[1]->Brush.SetResourceObject(PlusImage);
			}
			CommandImages[2]->Brush.SetResourceObject(SelectImage(command[1]));
		}
	}
}
UTexture2D* UCommandWidget::SelectImage(EInputKey key) {
	UTexture2D* Result = nullptr;

	switch (key) {
	case EInputKey::E_LEFTCLICK:
		Result = ImageArray[0];
		break;
	case EInputKey::E_RIGHTCLICK:
		Result = ImageArray[1];
		break;
	case EInputKey::E_FORWARD:
		Result = ImageArray[2];
		break;
	case EInputKey::E_BACKWARD:
		Result = ImageArray[3];
		break;
	case EInputKey::E_LEFT:
		Result = ImageArray[4];
		break;
	case EInputKey::E_RIGHT:
		Result = ImageArray[5];
		break;
	case EInputKey::E_EVADE:
		Result = ImageArray[6];
		break;
	}

	return Result;
}