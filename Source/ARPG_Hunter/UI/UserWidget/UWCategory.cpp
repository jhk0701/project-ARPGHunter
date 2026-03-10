// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWCategory.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"


void UUWCategoryElement::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CheckBox->OnCheckStateChanged.AddDynamic(this, &UUWCategoryElement::OnStateChanged);
}

void UUWCategoryElement::Init(uint8 _index, const FText& _text)
{
	Index = _index;
}

void UUWCategoryLabeledElement::Init(uint8 _index, const FText& _text)
{
	Super::Init(_index, _text);
	Label->SetText(_text);
}


void UUWCategoryElement::SetSelectedManually(bool _bIsOn)
{
	FOnCheckBoxComponentStateChanged Tmp = CheckBox->OnCheckStateChanged;
	CheckBox->OnCheckStateChanged.Clear();
	
	CheckBox->SetIsChecked(_bIsOn);

	CheckBox->OnCheckStateChanged = Tmp;
}

void UUWCategoryElement::OnStateChanged(bool _bIsOn)
{
	if (_bIsOn)
		OnSelected.ExecuteIfBound(Index);
}

void UUWCategory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (ElementClass == nullptr)
		return;

	ElementInst.SetNum(CategoryOptions.Num());

	for (uint8 i = 0; i < CategoryOptions.Num(); ++i)
	{
		const FCategoryOption& Option = CategoryOptions[i];

		TObjectPtr<UUWCategoryElement> Inst = CreateWidget<UUWCategoryElement>(GetWorld(), ElementClass);
		Inst->Init(Option.Value, Option.Text);
		Inst->OnSelected.BindUObject(this, &UUWCategory::SetElementSelected);

		Container->AddChild(Inst);
		ElementInst[i] = Inst;
	}

	SetElementSelected(CategoryOptions[0].Value);
}


void UUWCategory::SetElementSelected(uint8 _value)
{
	CurSelectedOption = _value;

	for (uint8 i = 0; i < CategoryOptions.Num(); ++i)
		ElementInst[i]->SetSelectedManually(CurSelectedOption == CategoryOptions[i].Value);

	OnSelected.Broadcast(CurSelectedOption);
}
