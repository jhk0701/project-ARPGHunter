// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeData.generated.h"

class USkillUpgrade;

USTRUCT()
struct FUpgradeInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkillUpgrade> Upgrade;
	UPROPERTY(EditAnywhere)
	uint8 Cost;
	/// 업그레이드를 반영할 액션의 인덱스
	UPROPERTY(EditAnywhere)
	uint8 TargetIndex;
};

USTRUCT()
struct FSkillNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FUpgradeInfo> UpgradeInfos; // 이 노드가 나타내는 업그레이드 기능

	UPROPERTY(EditAnywhere, Category = "Children")
	TArray<uint8> ChildrenIdx; // SkillTree의 배열에 할당된 노드들의 인덱스 사용
};

USTRUCT()
struct FSkillTree 
{
	GENERATED_BODY()

public:
	// 노드들은 전체적으로 하나의 배열로 관리
	// 각 노드에 자식에 해당하는 노드들의 포인터 대신 인덱스를 기입할 것
	UPROPERTY(EditAnywhere)
	TArray<FSkillNode> Tree;

	const FSkillNode* GetRoot() const 
	{ 
		if (Tree.IsEmpty())
			return nullptr;

		return &Tree[0]; 
	}
	const FSkillNode* GetNode(uint8 _idx) const 
	{ 
		if (Tree.Num() <= _idx)
			return nullptr;

		return &Tree[_idx]; 
	}
};

UCLASS()
class ARPG_HUNTER_API USkillTreeData : public UDataAsset
{
	GENERATED_BODY()
public:
	/// Key : ActionComboData 중, AttackAcionArray의 인덱스
	UPROPERTY(EditAnywhere)
	TMap<uint8, FSkillTree> SkillTrees;
};
