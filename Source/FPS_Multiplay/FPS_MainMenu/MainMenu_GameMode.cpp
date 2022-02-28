// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu_GameMode.h"
#include "MainMenuHUD.h"
#include "MainMenu_PlayerController.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayGameInstance.h"


AMainMenu_GameMode::AMainMenu_GameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
	HUDClass = AMainMenuHUD::StaticClass();
	PlayerControllerClass = AMainMenu_PlayerController::StaticClass();
}

void AMainMenu_GameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void AMainMenu_GameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (IsValid(CurrentWidget))
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (IsValid(NewWidgetClass))
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (IsValid(CurrentWidget))
        {
            CurrentWidget->AddToViewport();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AMainMenu_GameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) -> CurrentWidget is not Valid !!!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AMainMenu_GameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) -> NewWidgetClass is not Valid !!!"));
    }
}
