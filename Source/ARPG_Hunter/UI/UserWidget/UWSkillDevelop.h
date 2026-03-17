// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWSkillDevelop.generated.h"

class UButton;
class USkillTreeData;
class USkillUpgrade;

DECLARE_DELEGATE_RetVal_TwoParams(int8, FGetSkillUpgradeInfoFunc, uint8, uint8);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWSkillDevelop : public UUWPopUp
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<USkillTreeData> SkillTree; // 현재 설정 중인 스킬트리 약참조
	FGetSkillUpgradeInfoFunc GetSkillUpgradeInfoFunc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

protected:
	virtual void NativeOnInitialized() override;

public:
	void Init(TWeakObjectPtr<USkillTreeData> _skillTree, FGetSkillUpgradeInfoFunc& _func);
	bool IsValid() const { return SkillTree.IsValid() && GetSkillUpgradeInfoFunc.IsBound(); }

	void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;
};
