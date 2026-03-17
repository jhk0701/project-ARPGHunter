// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWSkillDevelop.generated.h"

class UButton;
class UTextBlock;
class UPanelWidget;
class UWrapBox;

class UActionComboData;
class USkillTreeData;
class USkillUpgrade;

DECLARE_DELEGATE_RetVal_TwoParams(int8, FGetSkillUpgradeInfoFunc, uint8, uint8);

UCLASS()
class ARPG_HUNTER_API UUWSkillNode : public UUserWidget 
{
	GENERATED_BODY()
private:
};

UCLASS()
class ARPG_HUNTER_API UUWSkillTree : public UUserWidget
{
	GENERATED_BODY()
private:
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
	void Construct(const struct FSkillTree* _tree, const TArray<uint8>& _level, const uint8 _height);
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWSkillDevelop : public UUWPopUp
{
	GENERATED_BODY()
private:
	TWeakObjectPtr<UActionComboData> ActionComboData;
	TWeakObjectPtr<USkillTreeData> SkillTreeData;
	FGetSkillUpgradeInfoFunc GetSkillUpgradeInfoFunc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWSkillTree> SkillTreeUIClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWSkillTree>> SkillTreeUIs;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> SkillTreeContainer;

protected:
	virtual void NativeOnInitialized() override;

public:
	void Init(TWeakObjectPtr<class UWeaponConfig> _curWeaponConfig, FGetSkillUpgradeInfoFunc& _func);
	bool IsValid() const;

	void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;
	void SetSkillTree();
};
