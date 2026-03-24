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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LineImage;

};

UCLASS()
class ARPG_HUNTER_API UUWSkillTree : public UUserWidget
{
	GENERATED_BODY()
public:
	struct FSkillNodeState 
	{
		uint8 Level{0}; // TreeLevel
		uint8 SiblingIdx{0};
		UUWSkillNode::EState State{ UUWSkillNode::EState::NONE };
	};
private:
	uint8 Index;
	struct FSkillTree* SkillTree;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> TreeContainer;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWSkillNode> SkillNodeClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWSkillNode>> SkillNodes;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWSkillNodeLine> SkillNodeLineClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWSkillNodeLine>> SkillNodeLines;

	UPROPERTY(EditAnywhere)
	float TopOffset{ 50.0f };
	UPROPERTY(EditAnywhere)
	float LeftOffset{ 200.0f };
	UPROPERTY(EditAnywhere)
	FVector2D NodeInterval{50.0, 50.0};
	UPROPERTY(EditAnywhere)
	FVector2D NodeSize{ 73.95, 85.0 };
	UPROPERTY(EditAnywhere)
	float LineWidth{ 3.0f };

public:
	FOnSkillNodeSelected OnSkillNodeSelected;
	void Construct(struct FSkillTree* _tree, const TArray<FSkillNodeState>& _treeNodeStates, const TArray<TArray<uint8>>& _siblingPerLevel, const uint8 _height);

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
