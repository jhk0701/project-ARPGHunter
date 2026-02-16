// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPlayerHUD.generated.h"

class UUWPlayerStatusBar;

/**
 * 만들어진 위젯을 배치하는 캔버스 용도의 위젯
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UUWPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
};

UCLASS()
class ARPG_HUNTER_API UUWCombatHUD : public UUWPlayerHUD
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWPlayerStatusBar> PlayerStatusBar;

public:
	UUWPlayerStatusBar* GetPlayerStatusBar() const { return PlayerStatusBar; }
};

UCLASS()
class ARPG_HUNTER_API UUWNonCombatHUD : public UUWPlayerHUD
{
	GENERATED_BODY()
};