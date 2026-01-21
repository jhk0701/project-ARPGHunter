// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS()
class ARPG_HUNTER_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMonsterBase();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStatComponent> StatComp;
	

protected:
	virtual void BeginPlay() override;

public:	
	// virtual void Tick(float DeltaTime) override;
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
