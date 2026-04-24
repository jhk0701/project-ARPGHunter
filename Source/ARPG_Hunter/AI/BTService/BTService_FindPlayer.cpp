// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_FindPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Monster/MonsterBase.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = TEXT("Find Player");
	Interval = 1.0f;
	bTickIntervals = true;
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	TObjectPtr<AMonsterBase> Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster == nullptr)
		return;

	FVector Loc = Monster->GetActorLocation();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	float RecogRange = BBComp->GetValueAsFloat(FName(TEXT("RecoginitionRange")));

	FHitResult HitResult;
	bool IsHit = UKismetSystemLibrary::SphereTraceSingle(
		Monster, 
		Loc, Loc,
		RecogRange,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3),
		false,
		{Monster},
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (IsHit)
	{
		BBComp->SetValueAsObject(FName(TEXT("Target")), HitResult.GetActor());
		// Monster->OnTargetFound();
	}
}
