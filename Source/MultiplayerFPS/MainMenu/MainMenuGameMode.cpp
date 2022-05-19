#include "MainMenuGameMode.h"
#include "MainMenuHUD.h"
#include "MainMenuPlayerController.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameInstance.h"


AMainMenuGameMode::AMainMenuGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
	HUDClass = AMainMenuHUD::StaticClass();
	PlayerControllerClass = AMainMenuPlayerController::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (IsValid(CurrentWidget))
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }

    if (!IsValid(NewWidgetClass))
    {
        UE_LOG(LogTemp, Warning, TEXT("AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) -> NewWidgetClass is not Valid !!!"));
        return;
    }

    CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
    if (!IsValid(CurrentWidget))
    {
        UE_LOG(LogTemp, Error, TEXT("AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) -> CurrentWidget is not Valid !!!"));
        return;
    }

    CurrentWidget->AddToViewport();
}
