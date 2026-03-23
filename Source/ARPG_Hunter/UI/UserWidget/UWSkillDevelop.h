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
DECLARE_DELEGATE_FourParams(FOnUpgradeClicked, uint8, uint8, uint8, uint8);

UCLASS()
class ARPG_HUNTER_API UUWSkillNode : public UUserWidget 
{
	GENERATED_BODY()
public:
	enum EState : uint8
	{
		NONE,
		IN_PROGRESS,
		DONE
	};

private:
	uint8 Index;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SkillButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SkillThumbnail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectedMark;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> StateMark;
	UPROPERTY(EditAnywhere)
	TArray<FSlateColor> ColorOnState;
	
	UFUNCTION()
	void ClickButton();

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnClickSkillNode OnClickSkillNode;
	void SetSkillThumbnail(TObjectPtr<UTexture2D> _tex);
	void SetIndex(uint8 _idx) { Index = _idx; };
	void SetState(EState _state);
	void SetSelected(bool _bIsSelected);
	void SetButtonEnable(bool _bIsEnable);
};


UCLASS()
class ARPG_HUNTER_API UUWSkillNodeLine : public UUserWidget 
{
	GENERATED_BODY()
private:
	struct FSkillTree* SkillTree;
	TArray<TObjectPtr<UUWSkillNode>>* SkillNodes;

	FVector2D GetWidgetPosition(TObjectPtr<UUserWidget> _widget, FVector2D _normalized) const;
	
protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

public:
	void Init(FSkillTree* _skillTree, TArray<TObjectPtr<UUWSkillNode>>* _nodeUIs) 
	{
		SkillTree = _skillTree;
		SkillNodes = _nodeUIs;
	}
};

UCLASS()
class ARPG_HUNTER_API UUWSkillTree : public UUserWidget
{
	GENERATED_BODY()
public:
	struct FSkillNodeState 
	{
		uint8 Level; // TreeLevel
		UUWSkillNode::EState State;
	};
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWSkillNodeLine> NodeLine;

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnSkillNodeSelected OnSkillNodeSelected;
	void Construct(struct FSkillTree* _tree, const TArray<FSkillNodeState>& _treeNodeStates, const uint8 _height);

	void SetIndex(uint8 _idx) { Index = _idx; }
	void SetSkillLabel(const FText& _name);
	void OnClickNode(uint8 _idx);
	void UpdateNode(uint8 _idx, UUWSkillNode::EState _state);
	
	TObjectPtr<UUWSkillNode> GetNodeUI(uint8 _nodeIdx) { return SkillNodes[_nodeIdx]; }
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
	int8 CurUpgrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWSkillTree> SkillTreeUIClass;
	UPROPERTY()
	TMap<uint8, TObjectPtr<UUWSkillTree>> SkillTreeUIs;
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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeLabel;
	UPROPERTY(EditAnywhere)
	FText FullUpgradeText;
	UPROPERTY(EditAnywhere)
	FText UpgradableText;

	void ShowDetail();
	void HideDetail();

	UFUNCTION()
	void ClickUpgrade();

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnUpgradeClicked OnUpgradeClicked;

	void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;

	void Init(TWeakObjectPtr<class UWeaponConfig> _curWeaponConfig, FGetSkillUpgradeInfoFunc&& _upgradeInfofunc, FGetUsableSkillPointFunc&& _usableSkillPointFunc);
	bool IsValid() const;
	void SetSkillTree();
	void UpdateSkillTree();
	void SelectSkillNode(uint8 _key, uint8 _nodeIdx);

	UUWSkillNode::EState GetNodeState(uint8 _key, uint8 _nodeIdx, int8 _upgradeLv);
};
