// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/StageSelector/StageSelector.h"
#include "Components/BoxComponent.h"

#include "Core/GameMode/ARPGGameMode.h"
#include "UI/UserWidget/UWStageSelect.h"

// Sets default values
AStageSelector::AStageSelector()
{
	PrimaryActorTick.bCanEverTick = false;

	ColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(ColliderComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(ColliderComp);

	static ConstructorHelpers::FClassFinder<UUWStageSelect> StageSelectUIFinder(TEXT("/Game/06-UI/WBP_StageSelect.WBP_StageSelect_C"));
	if (StageSelectUIFinder.Succeeded())
		StageSelectUIClass = StageSelectUIFinder.Class;
}

void AStageSelector::BeginPlay()
{
	Super::BeginPlay();

	if (StageSelectUIClass)
	{
		StageSelectUI = CreateWidget<UUWStageSelect>(GetWorld(), StageSelectUIClass);
		StageSelectUI->OnClickStartButton.BindUObject(this, &AStageSelector::StartGame);
	}
}

void AStageSelector::Interact()
{
	Super::Interact();

	// 데이터 테이블 기반 UI 띄우기
	StageSelectUI->ShowUI();
}

void AStageSelector::StartGame(const FName& _selectedID)
{
	GetWorld()->GetAuthGameMode<AARPGGameMode>()->OpenLevel(_selectedID);
}