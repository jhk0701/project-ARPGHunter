// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemSlotIndicate.h"
#include "Components/TextBlock.h"

void UUWItemSlotIndicate::SetMessage(const FText& _content)
{
	MessageLabel->SetText(_content);
}
