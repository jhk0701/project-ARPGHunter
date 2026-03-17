// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWSkillDevelop.generated.h"

class UButton;
class UTextBlock;
class UPanelWidget;
class UImage;
class UBorder;

class UActionComboData;
class USkillTreeData;
class USkillUpgrade;

DECLARE_DELEGATE_OneParam(FOnClickSkillNode, uint8);
DECLARE_DELEGATE_TwoParams(FOnSkillNodeSelected, uint8, uint8);
DECLARE_DELEGATE_RetVal_TwoParams(int8, FGetSkillUpgradeInfoFunc, uint8, uint8);
DECLARE_DELEGATE_RetVal(uint16, FGetUsableSkillPointFunc);

UCLASS()
class ARPG_HUNTER_API UUWSkillNode : public UUserWidget 
{
	GENERATED_BODY()
private:
	uint8 Index;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SkillButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SkillThumbnail;
	
	UFUNCTION()
	void ClickButton();

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnClickSkillNode OnClickSkillNode;

	void SetSkillThumbnail(TObjectPtr<UTexture2D> _tex);
	void SetIndex(uint8 _idx) { Index = _idx; };
};

UCLASS()
class ARPG_HUNTER_API UUWSkillTree : public UUserWidget
{
	GENERATED_BODY()
private:
	uint8 Index;
	struct FSkillTree* SkillTree;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SkillLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> TreeContainer;
	UPROPERTY()
	TArray<TObjectPtr<UPanelWidget>> LevelContainer;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWSkillNode> SkillNodeClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWSkillNode>> SkillNodes;

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnSkillNodeSelected OnSkillNodeSelected;
	void SetIndex(uint8 _idx) { Index = _idx; }
	void SetSkillLabel(const FText& _name);
	void Construct(struct FSkillTree* _tree, const TArray<uint8>& _level, const uint8 _height);
	void OnClickNode(uint8 _idx);
};

UCLASS()
class ARPG_HUNTER_API UUWSkillDevelop : public UUWPopUp
{
	GENERATED_BODY()
private:
	TWeakObjectPtr<UActionComboData> ActionComboData;
	TWeakObjectPtr<USkillTreeData> SkillTreeData;
	FGetSkillUpgradeInfoFunc GetSkillUpgradeInfoFunc;
	FGetUsableSkillPointFunc GetUsableSkillPointFunc;

	uint8 CurKey;
	uint8 CurNodeIdx;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWSkillTree> SkillTreeUIClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWSkillTree>> SkillTreeUIs;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> SkillTreeContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> SkillNodeDetail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NodeNameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NodeDescLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUWIngredientSlot> SkillPoint;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UpgradeButton;

	void ShowDetail();
	void HideDetail();

	UFUNCTION()
	void ClickUpgrade();

protected:
	virtual void NativeOnInitialized() override;

public:
	void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;

	void Init(TWeakObjectPtr<class UWeaponConfig> _curWeaponConfig, FGetSkillUpgradeInfoFunc& _upgradeInfofunc, FGetUsableSkillPointFunc& _usableSkillPointFunc);
	bool IsValid() const;
	void SetSkillTree();
	void SelectSkillNode(uint8 _key, uint8 _nodeIdx);
};
