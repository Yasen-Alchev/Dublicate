#include "CQ_GameMode.h"

#include "CQ_GameState.h"
#include "CQ_InGameHUD.h"
#include "CQ_PlayerController.h"
#include "CQ_PlayerState.h"
#include "Blueprint/UserWidget.h"

ACQ_GameMode::ACQ_GameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
    HUDClass = ACQ_InGameHUD::StaticClass();
    PlayerControllerClass = ACQ_PlayerController::StaticClass();
    PlayerStateClass = ACQ_PlayerState::StaticClass();
    GameStateClass = ACQ_GameState::StaticClass();

    minutes = 5;
    seconds = 0;
}

void ACQ_GameMode::BeginPlay()
{
    Super::BeginPlay();
}

