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


void UUWNPCDialog::Init(TArray<FNPCDialogOption>& _options)
{
	if (DialogOptionClass == nullptr)
		return;

	DialogOptionInst.SetNum(_options.Num());

	for (uint8 i = 0; i < _options.Num(); ++i)
	{
		TObjectPtr<UUWDialogOption> OptionInst = CreateWidget<UUWDialogOption>(GetWorld(), DialogOptionClass);
		DialogOptionInst[i] = OptionInst;

		FNPCDialogOption* pOpt = &_options[i];
		OptionInst->OnOptionClicked.BindLambda(
			[pOpt, this]()
			{
				if (GetUIFunc.IsBound() == false)
					return;

				if (TObjectPtr<UUWPopUp> UIInst = GetUIFunc.Execute(pOpt->UIClass))
					UIInst->ShowUI(true, this);
			}
		);

		OptionInst->SetButtonLabel(FText::FromString(_options[i].DialogTitle));
		DialogOptionContainer->AddChild(OptionInst);
	}

	CloseOption = CreateWidget<UUWDialogOption>(GetWorld(), DialogOptionClass);
	CloseOption->SetButtonLabel(FText::FromString(TEXT("대화 끝내기")));
	CloseOption->OnOptionClicked.BindUObject(this, &UUWNPCDialog::HideUI);
	DialogOptionContainer->AddChild(CloseOption);
}

void UUWNPCDialog::SetDialog(FDialogData* _firstDialogData)
{
	if (_firstDialogData == nullptr)
		return;

	NameLabel->SetText(FText::FromName(_firstDialogData->NPCName));
	DialogLabel->SetText(FText::FromString(_firstDialogData->Message));
}