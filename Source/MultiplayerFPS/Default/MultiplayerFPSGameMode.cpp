#include "MultiplayerFPSGameMode.h"

#include "EngineUtils.h"
#include "MultiplayerFPSGameState.h"
#include "MultiplayerFPSInGameHUD.h"
#include "MultiplayerFPSPlayerController.h"
#include "MultiplayerFPSPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/CommonClasses/PlayerStartPoint.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerFPSGameMode::AMultiplayerFPSGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
    HUDClass = AMultiplayerFPSInGameHUD::StaticClass();
    PlayerControllerClass = AMultiplayerFPSPlayerController::StaticClass();
    PlayerStateClass = AMultiplayerFPSPlayerState::StaticClass();
    GameStateClass = AMultiplayerFPSGameState::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bAllowTickOnDedicatedServer = true;

    minutes = 5;
    seconds = 0;
    minPlayersToStart = 2;
    bStarted = false;
}

void AMultiplayerFPSGameMode::BeginPlay()
{
    Super::BeginPlay();
    ChangeMenuWidget(StartingWidgetClass);

   /* if (bStarted)
    {
        GetWorldTimerManager().ClearTimer(GameTimer);
        GetWorldTimerManager().SetTimer(GameTimer, [this]()
            {
                UpdateGlobalGameTimer(minutes, seconds);
            }, 1, true, 0.f);
    }*/
}

void AMultiplayerFPSGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bStarted)
    {
        if (NumPlayers >= minPlayersToStart)
        {
            AMultiplayerFPSGameState* GameStateVar = GetGameState<AMultiplayerFPSGameState>();
            if (IsValid(GameStateVar))
            {
                bStarted = true;
                if (GetWorldTimerManager().IsTimerActive(GameTimer))
                {
                    GetWorldTimerManager().ClearTimer(GameTimer);
                }
                GetWorldTimerManager().SetTimer(GameTimer, [this, GameStateVar]()
                    {
                        GameStateVar->DisablePlayersControls(true);
                        StartingGame();
                    }, 1, false);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameMode::Tick(float DeltaSeconds) -> GameStateVar is not Valid !!!"));
            }
        }
    }
}

void AMultiplayerFPSGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AMultiplayerFPSGameMode::UpdateGlobalGameTimer(int& min, int& sec)
{
    AMultiplayerFPSGameState* GameStateVar = GetGameState<AMultiplayerFPSGameState>();
    if (IsValid(GameStateVar))
    {
        if (--sec <= 0)
        {
            sec = 59;
            if (--min == -1)
            {
                min = 0;
                sec = 0;
                if(GetWorldTimerManager().IsTimerActive(GameTimer))
                {
					GetWorldTimerManager().ClearTimer(GameTimer);
                }
                GameStateVar->GameEnded();
            }
        }
        GameStateVar->UpdateGameTime(min, sec);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameMode::UpdateGlobalGameTimer() -> GameStateVar is not Valid!!!"));
    }
}

void AMultiplayerFPSGameMode::UpdateObjectiveStats()
{
    AMultiplayerFPSGameState* GameStateVar = GetGameState<AMultiplayerFPSGameState>();
    if (IsValid(GameStateVar))
    {
        GameStateVar->UpdateObjectiveStats();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameMode::UpdateObjectiveStats() -> GameStateVar is not Valid!!!"));
    }
}

void AMultiplayerFPSGameMode::HandleStartingNewPlayer_Implementation(APlayerController* MovieSceneBlends)
{
    Super::HandleStartingNewPlayer_Implementation(MovieSceneBlends);
}

void AMultiplayerFPSGameMode::PostLogin(APlayerController* MovieSceneBlends)
{
    Super::PostLogin(MovieSceneBlends);
}

void AMultiplayerFPSGameMode::StartingGame()
{
    AMultiplayerFPSGameState* GameStateVar = GetGameState<AMultiplayerFPSGameState>();
    if (IsValid(GameStateVar))
    {
        GameStateVar->SetGlobalGameMessage("Game is Starting! Please Wait...");
        if (GetWorldTimerManager().IsTimerActive(StartingTimer))
        {
            GetWorldTimerManager().ClearTimer(StartingTimer);
        }
        GetWorldTimerManager().SetTimer(StartingTimer, [this, GameStateVar]()
            {
                GameStateVar->DisablePlayersControls(false);
                GameStateVar->RespawnPlayers(true);
                GameStateVar->ClearGlobalGameMessage();
				if(GetWorldTimerManager().IsTimerActive(GameTimer))
				{
                    GetWorldTimerManager().ClearTimer(GameTimer);
				}
                GetWorldTimerManager().SetTimer(GameTimer, [this]()
                    {
                    if(GetWorldTimerManager().IsTimerActive(StartingTimer))
                    {
                        GetWorldTimerManager().ClearTimer(StartingTimer);
                    }
                        UpdateGlobalGameTimer(minutes, seconds);
                    }, 1, true, 0.f);

            }, 1, false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameMode::StartingGame() -> GameStateVar is not Valid!!!"));
    }
}

AActor* AMultiplayerFPSGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends)
{
    Super::ChoosePlayerStart_Implementation(MovieSceneBlends);

    if (IsValid(MovieSceneBlends))
    {
        AMultiplayerFPSPlayerState* PlayerStateVariable = MovieSceneBlends->GetPlayerState<AMultiplayerFPSPlayerState>();
        if (IsValid(PlayerStateVariable))
        {
            TArray<APlayerStart*> Starts;
            for (TActorIterator<APlayerStart> StartItr(GetWorld()); StartItr; ++StartItr)
            {
                    Starts.Add(*StartItr);
            }
            if (Starts.Num() > 0)
            {
                return Starts[FMath::RandRange(0, Starts.Num() - 1)];
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> (Starts.Num() > 0) -> There are no Starts found !!!"), *MovieSceneBlends->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> PlayerStateVariable is not Valid !!!"), *MovieSceneBlends->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> MovieSceneBlends is not Valid !!!"), *MovieSceneBlends->GetName());
    }
    return nullptr;
}


