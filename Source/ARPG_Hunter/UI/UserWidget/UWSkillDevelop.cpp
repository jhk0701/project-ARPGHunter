// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWSkillDevelop.h"
#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
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

void UUWSkillNode::SetState(EState _state)
{
	SelectedMark->SetBrushTintColor(ColorOnState[static_cast<uint8>(_state)]);
}

void UUWSkillNode::SetButtonEnable(bool _bIsEnable)
{
	SkillButton->SetIsEnabled(_bIsEnable);
}


void UUWSkillTree::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const int32 CHILDREN_COUNT = TreeContainer->GetChildrenCount();
	LevelContainer.Reserve(CHILDREN_COUNT);
	for (int32 i = 0; i < CHILDREN_COUNT; ++i)
		LevelContainer.Add(Cast<UPanelWidget>(TreeContainer->GetChildAt(i)));
}

void UUWSkillTree::Construct(FSkillTree* _tree, const TArray<FSkillNodeState>& _treeNodeStates, const uint8 _height)
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
		NodeInst->SetState(_treeNodeStates[i].State);
		NodeInst->SetButtonEnable(false);
		NodeInst->OnClickSkillNode.BindUObject(this, &UUWSkillTree::OnClickNode);
		
		LevelContainer[_treeNodeStates[i].Level]->AddChild(NodeInst);
		SkillNodes.Add(NodeInst);
	}

	// ConnectLines();

	// 노드 상태에 따른 자식 노드 상호작용 설정
	SkillNodes[0]->SetButtonEnable(true);
	for (uint8 i = 0; i < SkillTree->Tree.Num(); ++i) 
	{
		if (_treeNodeStates[i].State == UUWSkillNode::NONE)
			continue;

		const FSkillNode* Node = SkillTree->GetNode(i);
		for (uint8 ChildIdx : Node->ChildrenIdx)
			SkillNodes[ChildIdx]->SetButtonEnable(true);
	}
}

void UUWSkillTree::ConnectLines()
{
	if (nullptr == SkillTree || nullptr == NodeLineClass)
		return;

	const FVector2D ALIGNMENT(0.5f);
	const FVector2D INIT_SIZE(3.0f, 10.0f);

	const int32 SIZE = SkillTree->Tree.Num();
	NodeLines.Reserve(SIZE * 2);

	for (uint8 i = 0; i < SIZE; ++i)
	{
		const FSkillNode* Node = SkillTree->GetNode(i);
		const FGeometry& CanvasGeo = LineContainer->GetCachedGeometry();
		FVector2D ParentLoc = SkillNodes[i]->GetCachedGeometry().GetAbsolutePosition();

		for (uint8 ChildIdx : Node->ChildrenIdx)
		{
			TObjectPtr<UUWSkillNodeLine> LineInst = CreateWidget<UUWSkillNodeLine>(GetWorld(), NodeLineClass);
			UCanvasPanelSlot* CanvasSlot = LineContainer->AddChildToCanvas(LineInst);
			CanvasSlot->SetAlignment(ALIGNMENT);
			CanvasSlot->SetSize(INIT_SIZE);

			FVector2D ChildLoc = SkillNodes[ChildIdx]->GetCachedGeometry().GetAbsolutePosition();
			FVector2D Dir = ChildLoc - ParentLoc;
			double Len = Dir.Length();
			Dir.Normalize();

			

			CanvasSlot->SetPosition(ParentLoc + Dir * Len * 0.5f);
		}
	}
}

void UUWSkillTree::SetSkillLabel(const FText& _name)
{
	SkillLabel->SetText(_name);
}

void UUWSkillTree::OnClickNode(uint8 _idx)
{
	OnSkillNodeSelected.ExecuteIfBound(Index, _idx);
}

void UUWSkillTree::UpdateNode(uint8 _idx, UUWSkillNode::EState _state)
{
	SkillNodes[_idx]->SetState(_state);

	if (_state != UUWSkillNode::NONE) 
	{
		const FSkillNode* Node = SkillTree->GetNode(_idx);
		for (uint8 ChildIdx : Node->ChildrenIdx)
			SkillNodes[ChildIdx]->SetButtonEnable(true);
	}
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
		TArray<UUWSkillTree::FSkillNodeState> TreeNodeState;
		TreeNodeState.SetNum(SkillTree.Value.Tree.Num());
		TreeNodeState[0].Level = 0;
		uint8 Height = 0;

		for (uint8 i = 0; i < SkillTree.Value.Tree.Num(); ++i)
		{
			const FSkillNode& Node = SkillTree.Value.Tree[i];
			for (uint8 Idx : Node.ChildrenIdx)
			{
				TreeNodeState[Idx].Level = TreeNodeState[i].Level + 1;
				Height = FMath::Max(Height, TreeNodeState[Idx].Level);
			}

			int8 UpgradeLv = GetSkillUpgradeInfoFunc.Execute(SkillTree.Key, i);
			TreeNodeState[i].State = GetNodeState(SkillTree.Key, i, UpgradeLv);
		}

		TObjectPtr<UUWSkillTree> UIInst = CreateWidget<UUWSkillTree>(GetWorld(), SkillTreeUIClass);
		UIInst->SetIndex(SkillTree.Key);
		UIInst->SetSkillLabel(ActionComboData->AttackAcionArray[SkillTree.Key]->NameText);
		UIInst->Construct(&SkillTree.Value, TreeNodeState, Height + 1);
		UIInst->OnSkillNodeSelected.BindUObject(this, &UUWSkillDevelop::SelectSkillNode);
		
		SkillTreeUIs.Add(SkillTree.Key, UIInst);
		SkillTreeContainer->AddChild(UIInst);
	}
}

void UUWSkillDevelop::SelectSkillNode(uint8 _key, uint8 _nodeIdx)
{
	CurKey = _key;
	CurNodeIdx = _nodeIdx;
	CurUpgrade = GetSkillUpgradeInfoFunc.Execute(_key, _nodeIdx);

	ShowDetail();
}

UUWSkillNode::EState UUWSkillDevelop::GetNodeState(uint8 _key, uint8 _nodeIdx, int8 _upgradeLv)
{
	if (false == IsValid())
		return UUWSkillNode::NONE;

	const FSkillNode* Node = SkillTreeData->SkillTrees[_key].GetNode(_nodeIdx);
	if (nullptr == Node)
		return UUWSkillNode::NONE;

	if (_upgradeLv < 0)
		return UUWSkillNode::NONE;
	else if (_upgradeLv == Node->UpgradeInfos.Num() - 1)
		return UUWSkillNode::DONE;
	else
		return UUWSkillNode::IN_PROGRESS;
}

void UUWSkillDevelop::UpdateSkillTree()
{
	CurUpgrade = GetSkillUpgradeInfoFunc.Execute(CurKey, CurNodeIdx);
	SkillTreeUIs[CurKey]->UpdateNode(CurNodeIdx, GetNodeState(CurKey, CurNodeIdx, CurUpgrade));

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

	const FSkillNode& CurSkillNode = SkillTreeData->SkillTrees[CurKey].Tree[CurNodeIdx];
	bool bIsFullUpgrade = CurSkillNode.UpgradeInfos.Num() - 1 == CurUpgrade;
	const FUpgradeInfo& UpgradeInfo = bIsFullUpgrade ? CurSkillNode.UpgradeInfos.Last() : CurSkillNode.UpgradeInfos[CurUpgrade + 1];
	
	NodeNameLabel->SetText(UpgradeInfo.Upgrade->NameText);
	NodeDescLabel->SetText(UpgradeInfo.Upgrade->DescText);

	if (bIsFullUpgrade)
	{
		// 최대 강화 처리
		UpgradeButton->SetIsEnabled(false);
		UpgradeLabel->SetText(FullUpgradeText);
		SkillPoint->SetAmountLabel(FText::FromString(TEXT("-")), true);
		return;
	}
	else
		UpgradeLabel->SetText(UpgradableText);

	uint16 UsageSkillPoint = GetUsableSkillPointFunc.Execute();
	FText FormatText = FText::FromString(TEXT("{0} / {1}"));
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
	uint8 Cost = SkillTreeData->SkillTrees[CurKey].Tree[CurNodeIdx].UpgradeInfos[CurUpgrade + 1].Cost;
	OnUpgradeClicked.ExecuteIfBound(CurKey, CurNodeIdx, CurUpgrade + 1, Cost);

	UpdateSkillTree();
}
