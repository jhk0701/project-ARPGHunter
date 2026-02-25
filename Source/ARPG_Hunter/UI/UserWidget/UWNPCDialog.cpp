// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWNPCDialog.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "Data/NPCConfig.h"

void UUWDialogOption::SetButtonLabel(const FText& _text)
{
	ButtonLabel->SetText(_text);
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
}

void UUWNPCDialog::SetDialogOption(const TArray<FNPCDialog>& _options)
{
	if (_options.Num() > DialogOptionCount) 
	{
		uint8 Diff = _options.Num() - DialogOptionCount;
		SupplyOptionInst(Diff);
		DialogOptionCount = _options.Num();
	}

	for (uint8 i = 0; i < _options.Num(); ++i)
	{
		DialogOptionInst[i]->SetVisibility(ESlateVisibility::Visible);
		DialogOptionInst[i]->SetButtonLabel(FText::FromString(_options[i].DialogTitle));
	}
}
