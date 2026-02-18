// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "UWCheckBox.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCheckBoxChanged, bool, uint8);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWCheckBox : public UCheckBox
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	uint8 Option;

public:
	FOnCheckBoxChanged OnCheckBoxChanged;

	void Init();
	void SetOption(uint8 _opt) { Option = _opt; }
	uint8 GetOption() const { return Option; }
	
	UFUNCTION()
	void ClickCheckBox(bool _bIsChecked);

	void UpdateStateWithoutEvent(bool _bIsChecked);
};
