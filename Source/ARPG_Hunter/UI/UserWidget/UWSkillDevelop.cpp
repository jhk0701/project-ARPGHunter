// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWSkillDevelop.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetTree.h"

#include "Data/WeaponConfig.h"
#include "Data/ActionComboData.h"
#include "Data/Action.h"
#include "Data/SkillTreeData.h"
#include "Data/SkillUpgrade.h"


void UUWSkillTree::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const int32 CHILDREN_COUNT = TreeContainer->GetChildrenCount();
	LevelContainer.Reserve(CHILDREN_COUNT);
	for (int32 i = 0; i < CHILDREN_COUNT; ++i)
		LevelContainer.Add(Cast<UPanelWidget>(TreeContainer->GetChildAt(i)));
}

void UUWSkillTree::Construct(const FSkillTree* _tree, const TArray<uint8>& _level, const uint8 _height)
{
	if (nullptr == SkillNodeClass)
		return;

	SkillNodes.Reserve(_tree->Tree.Num());
	for (uint8 i = 0; i < _tree->Tree.Num(); ++i)
	{
		TObjectPtr<UUWSkillNode> NodeInst = CreateWidget<UUWSkillNode>(GetWorld(), SkillNodeClass);
		LevelContainer[_level[i]]->AddChild(NodeInst);
		SkillNodes.Add(NodeInst);
	}
}


void UUWSkillDevelop::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWSkillDevelop::HideUI);
}

void UUWSkillDevelop::Init(TWeakObjectPtr<UWeaponConfig> _curWeaponConfig, FGetSkillUpgradeInfoFunc& _func)
{
	ActionComboData = _curWeaponConfig->AttackCombo;
	SkillTreeData = _curWeaponConfig->SkillTree;
	GetSkillUpgradeInfoFunc = _func;

	SetSkillTree();
}

bool UUWSkillDevelop::IsValid() const
{
	return ActionComboData.IsValid() &&
		SkillTreeData.IsValid() &&
		GetSkillUpgradeInfoFunc.IsBound();
}

void UUWSkillDevelop::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);
}

void UUWSkillDevelop::SetSkillTree()
{
	if (false == IsValid() ||
		nullptr == SkillTreeUIClass)
		return;

	SkillTreeUIs.Reserve(SkillTreeData->SkillTrees.Num());

	for (const TPair<uint8, FSkillTree>& SkillTree : SkillTreeData->SkillTrees)
	{
		TArray<uint8> TreeLevel;
		TreeLevel.SetNum(SkillTree.Value.Tree.Num());
		TreeLevel[0] = 0;
		uint8 Height = 0;

		for (uint8 i = 0; i < SkillTree.Value.Tree.Num(); ++i)
		{
			const FSkillNode& Node = SkillTree.Value.Tree[i];
			for (uint8 Idx : Node.ChildrenIdx)
			{
				TreeLevel[Idx] = TreeLevel[i] + 1;
				Height = FMath::Max(Height, TreeLevel[Idx]);
			}
		}

		TObjectPtr<UUWSkillTree> UIInst = CreateWidget<UUWSkillTree>(GetWorld(), SkillTreeUIClass);
		SkillTreeUIs.Add(UIInst);
		UIInst->Construct(&SkillTree.Value, TreeLevel, Height + 1);

		SkillTreeContainer->AddChild(UIInst);
	}
}
