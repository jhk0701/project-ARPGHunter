// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWSkillDevelop.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"

#include "Data/WeaponConfig.h"
#include "Data/ActionComboData.h"
#include "Data/Action.h"
#include "Data/SkillTreeData.h"
#include "Data/SkillUpgrade.h"
#include "UI/UserWidget/UWEquipmentUtilSlot.h"


void UUWSkillNode::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SkillButton->OnClicked.AddDynamic(this, &UUWSkillNode::ClickButton);
}

void UUWSkillNode::ClickButton()
{
	OnClickSkillNode.ExecuteIfBound(Index);
}

void UUWSkillNode::SetSkillThumbnail(TObjectPtr<UTexture2D> _tex)
{
	SkillThumbnail->SetBrushFromTexture(_tex);
}


void UUWSkillTree::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const int32 CHILDREN_COUNT = TreeContainer->GetChildrenCount();
	LevelContainer.Reserve(CHILDREN_COUNT);
	for (int32 i = 0; i < CHILDREN_COUNT; ++i)
		LevelContainer.Add(Cast<UPanelWidget>(TreeContainer->GetChildAt(i)));
}

void UUWSkillTree::SetSkillLabel(const FText& _name)
{
	SkillLabel->SetText(_name);
}

void UUWSkillTree::Construct(FSkillTree* _tree, const TArray<uint8>& _level, const uint8 _height)
{
	if (nullptr == _tree || nullptr == SkillNodeClass)
		return;

	SkillTree = _tree;
	SkillNodes.Reserve(SkillTree->Tree.Num());
	for (uint8 i = 0; i < SkillTree->Tree.Num(); ++i)
	{
		TObjectPtr<UUWSkillNode> NodeInst = CreateWidget<UUWSkillNode>(GetWorld(), SkillNodeClass);

		NodeInst->SetIndex(i);
		NodeInst->SetSkillThumbnail((*SkillTree->Tree[i].UpgradeInfos.begin()).Upgrade->Thumbnail);
		NodeInst->OnClickSkillNode.BindUObject(this, &UUWSkillTree::OnClickNode);
		
		LevelContainer[_level[i]]->AddChild(NodeInst);
		SkillNodes.Add(NodeInst);
	}
}

void UUWSkillTree::OnClickNode(uint8 _idx)
{
	OnSkillNodeSelected.ExecuteIfBound(Index, _idx);
}


void UUWSkillDevelop::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWSkillDevelop::HideUI);
	UpgradeButton->OnClicked.AddDynamic(this, &UUWSkillDevelop::ClickUpgrade);
}

void UUWSkillDevelop::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);
	HideDetail();
}

void UUWSkillDevelop::Init(TWeakObjectPtr<UWeaponConfig> _curWeaponConfig, FGetSkillUpgradeInfoFunc& _upgradeInfofunc, FGetUsableSkillPointFunc& _usableSkillPointFunc)
{
	ActionComboData = _curWeaponConfig->AttackCombo;
	SkillTreeData = _curWeaponConfig->SkillTree;
	GetSkillUpgradeInfoFunc = _upgradeInfofunc;
	GetUsableSkillPointFunc = _usableSkillPointFunc;

	SetSkillTree();
}

bool UUWSkillDevelop::IsValid() const
{
	return ActionComboData.IsValid() && SkillTreeData.IsValid() &&
		GetSkillUpgradeInfoFunc.IsBound() && GetUsableSkillPointFunc.IsBound();
}

void UUWSkillDevelop::SetSkillTree()
{
	if (false == IsValid() ||
		nullptr == SkillTreeUIClass)
		return;

	SkillTreeUIs.Reserve(SkillTreeData->SkillTrees.Num());

	for (TPair<uint8, FSkillTree>& SkillTree : SkillTreeData->SkillTrees)
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
		UIInst->SetIndex(SkillTree.Key);
		UIInst->SetSkillLabel(ActionComboData->AttackAcionArray[SkillTree.Key]->NameText);
		UIInst->Construct(&SkillTree.Value, TreeLevel, Height + 1);
		UIInst->OnSkillNodeSelected.BindUObject(this, &UUWSkillDevelop::SelectSkillNode);
		
		SkillTreeUIs.Add(UIInst);
		SkillTreeContainer->AddChild(UIInst);
	}
}

void UUWSkillDevelop::SelectSkillNode(uint8 _key, uint8 _nodeIdx)
{
	CurKey = _key;
	CurNodeIdx = _nodeIdx;

	ShowDetail();
}

void UUWSkillDevelop::ShowDetail()
{
	SkillNodeDetail->SetVisibility(ESlateVisibility::Visible);

	if (false == IsValid())
	{
		HideDetail();
		return;
	}

	int8 UpgradeIdx = GetSkillUpgradeInfoFunc.Execute(CurKey, CurNodeIdx);

	FUpgradeInfo& UpgradeInfo = UpgradeIdx < 0 ? 
		*SkillTreeData->SkillTrees[CurKey].Tree[CurNodeIdx].UpgradeInfos.begin() :
		SkillTreeData->SkillTrees[CurKey].Tree[CurNodeIdx].UpgradeInfos[UpgradeIdx];
	
	NodeNameLabel->SetText(UpgradeInfo.Upgrade->NameText);
	NodeDescLabel->SetText(UpgradeInfo.Upgrade->DescText);

	FText FormatText = FText::FromString(TEXT("{0} / {1}"));
	uint16 UsageSkillPoint = GetUsableSkillPointFunc.Execute();
	bool bIsEnable = UpgradeInfo.Cost <= UsageSkillPoint;
	SkillPoint->SetAmountLabel(FText::Format(FormatText, UpgradeInfo.Cost, UsageSkillPoint), bIsEnable);
	UpgradeButton->SetIsEnabled(bIsEnable);
}

void UUWSkillDevelop::HideDetail()
{
	SkillNodeDetail->SetVisibility(ESlateVisibility::Hidden);
}

void UUWSkillDevelop::ClickUpgrade()
{
}
