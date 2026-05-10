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
	void InitInputGuideUI();
	virtual void InitMenuUI();
	TWeakObjectPtr<UUWPopUp> GetGameMenuUI() const;

	template<typename T>
	bool TryCreateWidget(TSubclassOf<T> _class, TObjectPtr<T>& _inst);
};

template<typename T>
inline bool APlayerHUD::TryCreateWidget(TSubclassOf<T> _class, TObjectPtr<T>& _inst)
{
	if (nullptr == _class)
		return false;

	_inst = CreateWidget<T>(GetWorld(), _class);

	return nullptr != _inst;
}
