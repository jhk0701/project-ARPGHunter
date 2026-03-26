// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/NPC/NPC.h"
#include "Components/CapsuleComponent.h"

#include "Core/Subsystem/DataManager.h"
#include "Data/NPCConfig.h"
#include "Data/DialogData.h"
#include "UI/UserWidget/UWNPCDialog.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = false;

	ColliderComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	SetRootComponent(ColliderComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(ColliderComp);

	// 기본값으로 설정
	// 레벨에 배치하고 변경해서 사용
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
	
	if (nullptr == Config)
		return;
	
	MeshComp->SetSkeletalMesh(Config->Mesh);
	MeshComp->SetAnimInstanceClass(Config->AnimInstClass);

	if (DialogUIClass)
	{
		DialogUI = CreateWidget<UUWNPCDialog>(GetWorld(), DialogUIClass);
		DialogUI->GetUIFunc.BindUObject(this, &ANPC::GetUI);
		DialogUI->Init(Config->DialogOptions);
	}
	
	for (const FNPCDialogOption& Dialog : Config->DialogOptions)
	{
		if (nullptr == Dialog.UIClass)
			continue;

		TObjectPtr<UUWPopUp> UIInstance = CreateWidget<UUWPopUp>(GetWorld(), Dialog.UIClass);
		MapUIInstance.Add(Dialog.UIClass, UIInstance);
	}
}

void ANPC::Interact()
{
	Super::Interact();

	if (nullptr == DialogUI || nullptr == Config)
		return;

	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	DialogUI->SetDialog(DataManager->GetDialogData(Config->FirstDialogID));
	DialogUI->ShowUI();
}
