// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/GameMode/Title/TitleGameMode.h"

#include "Controller/Player/PlayerCharacterController.h"
#include "Core/Subsystem/PlayerManager/PlayerManager.h"
#include "Core/Subsystem/SaveLoadManager/SaveLoadManager.h"
#include "SaveGame/PlayerData/PlayerSaveGame.h"

#include "UI/UserWidget/UWTitleScreen.h"

ATitleGameMode::ATitleGameMode()
{
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
	PlayerControllerClass = APlayerCharacterController::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> TitleUIFinder(TEXT("/Game/06-UI/WBP_Title.WBP_Title_C"));
	if (TitleUIFinder.Succeeded())
		TitleUIClass = TitleUIFinder.Class;
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (TitleUIClass == nullptr)
		return;

	TitleUIInst = CreateWidget<UUserWidget>(GetWorld(), TitleUIClass);
	TObjectPtr<UUWTitleScreen> TitleUI = Cast<UUWTitleScreen>(TitleUIInst);
	if (TitleUI == nullptr)
		return;

	TitleUI->OnClickNewGame.BindUObject(this, &ATitleGameMode::ClickNewGame);
	TitleUI->OnClickContinue.BindUObject(this, &ATitleGameMode::ClickContinue);
	TitleUI->OnClickExit.BindUObject(this, &ATitleGameMode::ExitGame);

	TitleUI->ShowContinueButton(GetGameInstance()->GetSubsystem<USaveLoadManager>()->DoesDataExist<UPlayerSaveGame>());
	TitleUI->ShowUI();
}

void ATitleGameMode::ClickNewGame()
{
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TObjectPtr<UUWTitleScreen> TitleUI = Cast<UUWTitleScreen>(TitleUIInst);

	// PlayerManager 초기화
	PlayerManager->CreateNewPlayer(TitleUI->GetInputName());
	
	TitleUI->HideUI();
	GoToTown();
}

void ATitleGameMode::ClickContinue()
{
	TObjectPtr<USaveLoadManager> SaveLoad = GetGameInstance()->GetSubsystem<USaveLoadManager>();

	FOnSaveLoadComplete Callback;
	Callback.BindWeakLambda(this, 
		[this]() 
		{
			TObjectPtr<UUWTitleScreen> TitleUI = Cast<UUWTitleScreen>(TitleUIInst);
			TitleUI->HideUI();
			GoToTown();
		}
	);

	SaveLoad->LoadGame<UPlayerSaveGame>(Callback);
}
