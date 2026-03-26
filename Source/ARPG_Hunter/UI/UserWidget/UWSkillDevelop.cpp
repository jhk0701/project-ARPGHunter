// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWSkillDevelop.h"
#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Overlay.h"

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
	
	SetState(EState::NONE);
	SetSelected(false);
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
	StateMark->SetBrushTintColor(ColorOnState[static_cast<uint8>(_state)]);
}

void UUWSkillNode::SetSelected(bool _bIsSelected)
{
	SelectedMark->SetVisibility(_bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UUWSkillNode::SetButtonEnable(bool _bIsEnable)
{
	SkillButton->SetIsEnabled(_bIsEnable);
}


void UUWSkillTree::Construct(FSkillTree* _tree, const TArray<FSkillNodeState>& _treeNodeStates, const TArray<TArray<uint8>>& _siblingPerLevel, const uint8 _height)
{
	if (nullptr == _tree || nullptr == SkillNodeClass)
		return;

	SkillTree = _tree;
	const uint8 NODE_CNT = SkillTree->Tree.Num();
	SkillNodes.Reserve(NODE_CNT);
	SkillNodeLines.Reserve(NODE_CNT);

	for (uint8 i = 0; i < NODE_CNT; ++i)
	{
		TObjectPtr<UUWSkillNode> NodeInst = CreateWidget<UUWSkillNode>(GetWorld(), SkillNodeClass);
		NodeInst->SetIndex(i);
		NodeInst->SetSkillThumbnail((*SkillTree->Tree[i].UpgradeInfos.begin()).Upgrade->Thumbnail);
		NodeInst->SetState(_treeNodeStates[i].State);
		NodeInst->SetButtonEnable(false);
		NodeInst->OnClickSkillNode.BindUObject(this, &UUWSkillTree::OnClickNode);
		
		SkillNodes.Add(NodeInst);
		
		// 캔버스 상 위치 배치
		UCanvasPanelSlot* NodeCanvasSlot = TreeContainer->AddChildToCanvas(NodeInst);
		NodeCanvasSlot->SetSize(NodeSize);
		NodeCanvasSlot->SetAlignment({ 0.5, 0.5 });
		
		uint8 SiblingCnt = _siblingPerLevel[_treeNodeStates[i].Level].Num();
		FVector2D Position
		{ 
			LeftOffset - (SiblingCnt - 1) * NodeInterval.X * 0.5f,
			TopOffset + _treeNodeStates[i].Level * NodeInterval.Y 
		};

		Position.X += _treeNodeStates[i].SiblingIdx * NodeInterval.X;
		Position.Y += _treeNodeStates[i].Level * NodeSize.Y;
		NodeCanvasSlot->SetPosition(Position);
	}

	const float NODE_Y_HALF = NodeSize.Y * 0.5f;
	// 선 배치
	const FVector2D DOWN{ 0, 1.0 };
	for (uint8 i = 0; i < NODE_CNT; ++i)
	{
		UCanvasPanelSlot* ParentCanvasSlot = Cast<UCanvasPanelSlot>(SkillNodes[i]->Slot);
		FVector2D ParentPos = ParentCanvasSlot->GetPosition() + FVector2D(0.0, NODE_Y_HALF);

		for (uint8 ChildIdx : SkillTree->Tree[i].ChildrenIdx)
		{
			UCanvasPanelSlot* ChildCanvasSlot = Cast<UCanvasPanelSlot>(SkillNodes[ChildIdx]->Slot);
			FVector2D ChildPos = ChildCanvasSlot->GetPosition();
			ChildPos.Y -= NODE_Y_HALF;

			TObjectPtr<UUWSkillNodeLine> LineInst = CreateWidget<UUWSkillNodeLine>(GetWorld(), SkillNodeLineClass);
			SkillNodeLines.Add(LineInst);

			LineInst->SetRenderTransformPivot({ 0.5, 0.0 });
			UCanvasPanelSlot* LineCanvasSlot = TreeContainer->AddChildToCanvas(LineInst);
			LineCanvasSlot->SetAlignment({ 0.5, 0.0 });

			FVector2D vecP2C = ChildPos - ParentPos;
			LineCanvasSlot->SetSize({ LineWidth, vecP2C.Length() });
			LineCanvasSlot->SetPosition(ParentPos);

			vecP2C.Normalize();
			double crs = FVector2D::CrossProduct(DOWN, vecP2C);
			LineInst->SetRenderTransformAngle(FMath::RadiansToDegrees(FMath::Acos(vecP2C.Dot(DOWN))) * (crs > 0 ? 1 : -1));
		}
	}

	// 노드 상태에 따른 자식 노드 상호작용 설정
	SkillNodes[0]->SetButtonEnable(true);
	for (uint8 i = 0; i < SkillTree->Tree.Num(); ++i) 
	{
		const FSkillNode* Node = SkillTree->GetNode(i);
		bool bIsNodeEnable = _treeNodeStates[i].State != UUWSkillNode::NONE;
		
		if (Node->ChildrenIdx.Num() <= 0)
			continue;
		
		for (uint8 j = 0; j < Node->ChildrenIdx.Num(); ++j)
			SkillNodes[Node->ChildrenIdx[j]]->SetButtonEnable(bIsNodeEnable);
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

	if (nullptr != SkillTreeUIs.Find(CurKey))
		SkillTreeUIs[CurKey]->GetNodeUI(CurNodeIdx)->SetSelected(false); // 이전 항목 선택 해제
	
	HideDetail();
}

void UUWSkillDevelop::Init(TWeakObjectPtr<UWeaponConfig> _curWeaponConfig, FGetSkillUpgradeInfoFunc&& _upgradeInfofunc, FGetUsableSkillPointFunc&& _usableSkillPointFunc)
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
		TArray<TArray<uint8>> SiblingPerLevel;
		uint8 Height = 0;

		const int32 CHILD_CNT = SkillTree.Value.Tree.Num();
		TreeNodeState.SetNum(CHILD_CNT);
		TreeNodeState[0].Level = 0;

		for (uint8 i = 0; i < CHILD_CNT; ++i)
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

		SiblingPerLevel.SetNumZeroed(Height + 1);
		for (uint8 i = 0; i < CHILD_CNT; ++i)
		{
			uint8 lv = TreeNodeState[i].Level;
			if (SiblingPerLevel[lv].Num() == 0)
				SiblingPerLevel[lv].Reserve(CHILD_CNT);
			
			SiblingPerLevel[lv].Add(i);
			TreeNodeState[i].SiblingIdx = SiblingPerLevel[lv].Num() - 1;
		}

		TObjectPtr<UUWSkillTree> UIInst = CreateWidget<UUWSkillTree>(GetWorld(), SkillTreeUIClass);
		UIInst->SetIndex(SkillTree.Key);
		UIInst->SetSkillLabel(ActionComboData->AttackAcionArray[SkillTree.Key]->NameText);
		UIInst->Construct(&SkillTree.Value, TreeNodeState, SiblingPerLevel, Height + 1);
		UIInst->OnSkillNodeSelected.BindUObject(this, &UUWSkillDevelop::SelectSkillNode);
		
		SkillTreeUIs.Add(SkillTree.Key, UIInst);
		SkillTreeContainer->AddChild(UIInst);
	}
}

void UUWSkillDevelop::SelectSkillNode(uint8 _key, uint8 _nodeIdx)
{
	if(nullptr != SkillTreeUIs.Find(CurKey))
		SkillTreeUIs[CurKey]->GetNodeUI(CurNodeIdx)->SetSelected(false); // 이전 항목 선택 해제

	CurKey = _key;
	CurNodeIdx = _nodeIdx;
	CurUpgrade = GetSkillUpgradeInfoFunc.Execute(_key, _nodeIdx);

	if (nullptr != SkillTreeUIs.Find(CurKey))
		SkillTreeUIs[CurKey]->GetNodeUI(CurNodeIdx)->SetSelected(true); // 새 항목 선택

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
	NodeDescLabel->SetText(FText::FromString(UpgradeInfo.Upgrade->DescString.Replace(TEXT("\\n"), TEXT("\n"))));

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