// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UserWidget/UWNPCDialog.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"

#include "Data/NPCConfig.h"
#include "Data/DialogData.h"

void UUWDialogOption::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button->OnClicked.AddDynamic(this, &UUWDialogOption::ClickOption);
}

void UUWDialogOption::SetButtonLabel(const FText& _text)
{
	ButtonLabel->SetText(_text);
}

void UUWDialogOption::ClickOption()
{
	OnOptionClicked.ExecuteIfBound();
}

void UUWNPCDialog::SupplyOptionInst(uint8 _amount)
{
	for (uint8 i = 0; i < _amount; ++i)
	{
		TObjectPtr<UUWDialogOption> Inst = CreateWidget<UUWDialogOption>(GetWorld(), DialogOptionClass);
		DialogOptionInst.Add(Inst);
		DialogOptionContainer->AddChild(Inst);
		Inst->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUWNPCDialog::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (DialogOptionClass == nullptr)
		return;

	DialogOptionInst.SetNum(DialogOptionCount);

	for (uint8 i = 0; i < DialogOptionCount; ++i)
	{
		DialogOptionInst[i] = CreateWidget<UUWDialogOption>(GetWorld(), DialogOptionClass);
		DialogOptionContainer->AddChild(DialogOptionInst[i]);
		DialogOptionInst[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	CloseOption = CreateWidget<UUWDialogOption>(GetWorld(), DialogOptionClass);
	CloseOption->SetButtonLabel(FText::FromString(TEXT("대화 끝내기")));
	CloseOption->OnOptionClicked.BindUObject(this, &UUWNPCDialog::HideUI);
}

void UUWNPCDialog::HideUI()
{
	Super::HideUI();
	
	CloseOption->SetVisibility(ESlateVisibility::Collapsed);
	DialogOptionContainer->RemoveChild(CloseOption);
}

void UUWNPCDialog::Init(FDialogData* _dialogData, const TArray<FNPCDialogOption>& _options)
{
	if (_dialogData == nullptr)
		return;

	NameLabel->SetText(FText::FromName(_dialogData->NPCName));
	DialogLabel->SetText(FText::FromString(_dialogData->Message));

	uint8 i = 0;
	for (i = 0; i < _options.Num(); ++i)
	{
		DialogOptionInst[i]->SetVisibility(ESlateVisibility::Visible);
		DialogOptionInst[i]->SetButtonLabel(FText::FromString(_options[i].DialogTitle));
	}

	CloseOption->SetVisibility(ESlateVisibility::Visible);
	DialogOptionContainer->AddChild(CloseOption);
}