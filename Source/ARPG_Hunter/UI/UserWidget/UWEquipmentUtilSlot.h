#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWEquipmentUtilSlot.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UItem;
struct FItemData;

DECLARE_DELEGATE_OneParam(FOnIndexedSlotClicked, uint8);

UCLASS()
class ARPG_HUNTER_API UUWEquipmentUtilSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameLabel;

	UPROPERTY(EditAnywhere, Category = "Assign")
	TObjectPtr<UTexture2D> AssignedThumbnail;
	UPROPERTY(EditAnywhere, Category = "Assign")
	FText AssignedName;

public:
	virtual void SynchronizeProperties() override;
	virtual void SetSlot(const TWeakObjectPtr<UItem> _item, const TArray<FText>* _addictiveText = nullptr);
	virtual void SetSlot(const FItemData* _itemData, const TArray<FText>* _addictiveText = nullptr);
	virtual void Clear();
};

UCLASS()
class ARPG_HUNTER_API UUWListElementSlot : public UUWEquipmentUtilSlot
{
	GENERATED_BODY()
private:
	uint8 Index;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SlotButton;

protected:
	void NativeOnInitialized() override;

public:
	FOnIndexedSlotClicked OnSlotClicked;

	void SetIndex(uint8 _idx) { Index = _idx; }

	UFUNCTION()
	void ClickSlot();
};

UCLASS()
class ARPG_HUNTER_API UUWIngredientSlot : public UUWEquipmentUtilSlot
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FColor ColorOnSufficient;
	UPROPERTY(EditAnywhere)
	FColor ColorOnInsufficient;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmountLabel;

public:
	virtual void SetSlot(const FItemData* _itemData, const TArray<FText>* _addictiveText = nullptr) override;
	void SetAmountLabel(const FText& _text, bool _bIsSufficient);
};