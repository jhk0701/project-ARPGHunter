// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWPoolingContainer.h"
#include "Components/PanelWidget.h"

void UUWPoolingContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotInstances.Reserve(InitPoolingSize);

	for (uint8 i = 0; i < InitPoolingSize; ++i)
		CreateSlot();
}


TObjectPtr<UUserWidget> UUWPoolingContainer::CreateSlot()
{
	if (SlotClass == nullptr)
		return nullptr;

	TObjectPtr<UUserWidget> Inst = CreateWidget<UUserWidget>(GetWorld(), SlotClass);
	Inst->SetVisibility(ESlateVisibility::Collapsed);
	
	SlotInstances.Push(Inst);
	SlotPool.Enqueue(Inst);

	Container->AddChild(Inst);

	return Inst;
}

TWeakObjectPtr<UUserWidget> UUWPoolingContainer::GetSlot()
{
	TWeakObjectPtr<UUserWidget> Inst;

	if (SlotPool.IsEmpty())
		CreateSlot();

	SlotPool.Dequeue(Inst);
	Inst->SetVisibility(ESlateVisibility::Visible);

	return Inst;
}

void UUWPoolingContainer::ReleaseSlot(TWeakObjectPtr<UUserWidget> _slot)
{
	_slot->SetVisibility(ESlateVisibility::Collapsed);
	SlotPool.Enqueue(_slot);
}