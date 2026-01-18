// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 MaxHealth{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 MaxStamina{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 StaminaRecovery{ 10 };
	// UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	// uint16 Strength{ 10 };
	// UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	// uint16 Agility{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 Attack{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 Defense{ 10 };

	UPROPERTY(VisibleAnywhere)
	uint16 Health{ 100 };
	UPROPERTY(VisibleAnywhere)
	uint16 Stamina{ 100 };

public:	
	UStatComponent();

protected:
	virtual void BeginPlay() override;
};
