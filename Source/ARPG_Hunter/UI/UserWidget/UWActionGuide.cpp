// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWActionGuide.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Define/Enum.h"
#include "Data/Action.h"
#include "Data/ActionComboData.h"
#include "Action/ActionInstance.h"
#include "Component/ActionComponent/PlayerActionComponent.h"


void UUWActionInfo::SetInfo(EAttackType _type, const FText& _name)
{
	InputKeyImage->SetBrushFromTexture(MapTypeInput[_type]);
	InputKeyImage->SetBrushTintColor(ColorOnUnlock);
	ActionNameLabel->SetText(_name);
}

void UUWActionInfo::SetLocked(EAttackType _type)
{
	InputKeyImage->SetBrushFromTexture(MapTypeInput[_type]);
	InputKeyImage->SetBrushTintColor(ColorOnLock);
	ActionNameLabel->SetText(LockText);
}

void UUWActionGuide::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(ActionInfoClass)
	{
		uint8 Max = static_cast<uint8>(EAttackType::END);
		NextActions.Reserve(Max);

		for (uint8 i = 0; i < Max; ++i)
		{
			TObjectPtr<UUWActionInfo> Inst = CreateWidget<UUWActionInfo>(GetWorld(), ActionInfoClass);
			NextActionContainer->AddChild(Inst);//Inst
			NextActions.Add(Inst);
		}
	}

	Clear();
}

void UUWActionGuide::SetActionInfo(bool _bIsInit, int8 _curIdx, const FAppliedGraph* _comboData)
{
	if (_comboData->Graph.Num() <= _curIdx)
	{
		Clear();
		return;
	}

	if (_bIsInit) 
		CurAction->SetVisibility(ESlateVisibility::Hidden);
	else
	{
		TObjectPtr<UAction> Action = _comboData->Actions[_curIdx]->GetAction();
		
		CurAction->SetInfo(Action->Type, Action->NameText);
		CurAction->SetVisibility(ESlateVisibility::Visible);
	}

	const TMap<EAttackType, FActionConnect>& Graph = _bIsInit ? _comboData->GraphStart : _comboData->Graph[_curIdx];
	for (uint8 i = 0; i < NextActions.Num(); ++i)
	{
		EAttackType Type = static_cast<EAttackType>(i);
		const FActionConnect* Connect = Graph.Find(Type);
		if (nullptr == Connect)
		{
			NextActions[i]->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}

		NextActions[i]->SetVisibility(ESlateVisibility::Visible);
		if (false == Connect->bIsUnlocked)
			NextActions[i]->SetLocked(Type);
		else
			NextActions[i]->SetInfo(Type, _comboData->Actions[Connect->Index]->GetAction()->NameText);
	}
}

void UUWActionGuide::Clear()
{
	CurAction->SetVisibility(ESlateVisibility::Hidden);

	for (const TObjectPtr<UUWActionInfo> Inst : NextActions)
		Inst->SetVisibility(ESlateVisibility::Collapsed);
}
