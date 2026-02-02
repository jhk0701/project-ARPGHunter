// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageSelector.h"
#include "Components/BoxComponent.h"

#include "GameMode/ARPGGameMode.h"

// Sets default values
AStageSelector::AStageSelector()
{
	PrimaryActorTick.bCanEverTick = false;

	ColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(ColliderComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(ColliderComp);
}

void AStageSelector::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Interact"));

	// TODO : 데이터 테이블 기반 UI 띄우기
	GetWorld()->GetAuthGameMode<AARPGGameMode>()->OpenLevel(FName(TEXT("Stage1")));
}
