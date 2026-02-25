// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NPC.h"
#include "Components/CapsuleComponent.h"

#include "Data/NPCConfig.h"
#include "UI/UserWidget/UWNPCDialog.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = false;

	ColliderComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	SetRootComponent(ColliderComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(ColliderComp);

	static ConstructorHelpers::FObjectFinder<UNPCConfig> ConfigFinder(TEXT("/Script/ARPG_Hunter.NPCConfig'/Game/03-Data/NPCConfig/NPC_Seller.NPC_Seller'"));
	if (ConfigFinder.Succeeded())
		Config = ConfigFinder.Object;

	static ConstructorHelpers::FClassFinder<UUWNPCDialog> DialogUIFinder(TEXT("/Game/06-UI/WBP_NpcDialog.WBP_NpcDialog_C"));
	if (DialogUIFinder.Succeeded())
		DialogUIClass = DialogUIFinder.Class;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	if (Config == nullptr)
		return;
	
	MeshComp->SetSkeletalMesh(Config->Mesh);
	MeshComp->SetAnimInstanceClass(Config->AnimInstClass);

	if (DialogUIClass)
		DialogUI = CreateWidget<UUWNPCDialog>(GetWorld(), DialogUIClass);
}

void ANPC::Interact()
{
	if(DialogUI)
		DialogUI->ShowUI();
}

