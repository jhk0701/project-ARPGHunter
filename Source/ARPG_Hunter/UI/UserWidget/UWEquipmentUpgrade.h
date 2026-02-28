// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWEquipmentUpgrade.generated.h"

class UUWProductSlot;
class UUWIngredientSlot;
class UUWStatChangeCompare;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipmentUpgrade : public UUWPopUp
{
	GENERATED_BODY()
	
private:
	TSubclassOf<UUWProductSlot> ListSlotClass;

public:

};
