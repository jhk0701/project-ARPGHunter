// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWSkillDevelop.h"
#include "Components/Button.h"

#include "Data/SkillTreeData.h"
#include "Data/SkillUpgrade.h"

void UUWSkillDevelop::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWSkillDevelop::HideUI);
}

void UUWSkillDevelop::Init(TWeakObjectPtr<USkillTreeData> _skillTree, FGetSkillUpgradeInfoFunc& _func)
{
	SkillTree = _skillTree;
	GetSkillUpgradeInfoFunc = _func;
}

void UUWSkillDevelop::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);
}
