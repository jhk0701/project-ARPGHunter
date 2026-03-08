// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPoolingContainer.generated.h"

UCLASS()
class ARPG_HUNTER_API UUWPoolingContainer : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Pooling")
	uint8 InitPoolingSize{30};
	UPROPERTY(EditAnywhere, Category = "Pooling")
	TSubclassOf<UUserWidget> SlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> SlotInstances; 
	TQueue<TWeakObjectPtr<UUserWidget>> SlotPool;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UPanelWidget> Container;

protected:
	virtual void NativeOnInitialized() override;

public:
	TObjectPtr<UUserWidget> CreateSlot();
	TWeakObjectPtr<UUserWidget> GetSlot();
	void ReleaseSlot(TWeakObjectPtr<UUserWidget> _slot);
};
