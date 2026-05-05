// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UUWPopUp;

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWPopUp> InputGuideUIClass;
	UPROPERTY()
	TObjectPtr<UUWPopUp> InputGuideUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWPopUp> GameMenuUIClass;
	UPROPERTY()
	TObjectPtr<UUWPopUp> GameMenuUI;

public:
	void ToggleGameMenuUI();
	void ToggleInputGuideUI();

protected:
	void BeginPlay() override;
	TWeakObjectPtr<UUWPopUp> GetGameMenuUI() const;

	virtual void InitMenuUI() {};

	template<typename UWType>
	bool TryCreateUI(TSubclassOf<UWType> _subClass, TObjectPtr<UWType> _outInst);
};

template<typename UWType>
inline bool APlayerHUD::TryCreateUI(TSubclassOf<UWType> _subClass, TObjectPtr<UWType> _outInst)
{
	if (nullptr == _subClass)
		return false;

	_outInst = CreateWidget<UWType>(GetWorld(), _subClass);
	return nullptr != _outInst;
}
