// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWInteractionIndicator.h"
#include "Components/TextBlock.h"

void UUWInteractionIndicator::SetText(const FText& _interactLabel)
{
	InteractLabel->SetText(_interactLabel);
}
