// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPS_MultiplayGameMode.h"

#include "EngineUtils.h"
#include "FPS_MultiplayTeamBasedCharacter.h"
#include "FPS_MultiplayGameState.h"
#include "FPS_MultiplayInGameHUD.h"
#include "FPS_MultiplayPlayerController.h"
#include "FPS_MultiplayPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

AFPS_MultiplayGameMode::AFPS_MultiplayGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
    HUDClass = AFPS_MultiplayInGameHUD::StaticClass();
    PlayerControllerClass = AFPS_MultiplayPlayerController::StaticClass();
    PlayerStateClass = AFPS_MultiplayPlayerState::StaticClass();
    GameStateClass = AFPS_MultiplayGameState::StaticClass();

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bAllowTickOnDedicatedServer = true;

    redPlayers = 0;
    bluePlayers = 0;
    minutes = 5;
    seconds = 0;
    minPlayersToStart = 2;
    bStarted = false;
}

void AFPS_MultiplayGameMode::BeginPlay()
{
    Super::BeginPlay();
    ChangeMenuWidget(StartingWidgetClass);

    if (bStarted)
    {
        GetWorldTimerManager().SetTimer(GameTimer, [this]()
            {
                UpdateGlobalGameTimer(minutes, seconds);
            }, 1, true, 0.f);
    }
}

void AFPS_MultiplayGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bStarted)
    {
        if (NumPlayers >= minPlayersToStart)
        {
            AFPS_MultiplayGameState* GameStateVar = GetGameState<AFPS_MultiplayGameState>();
            if (IsValid(GameStateVar))
            {
                bStarted = true;
                GetWorldTimerManager().SetTimer(GameTimer, [this, GameStateVar]()
                    {
                        GameStateVar->DisablePlayersControls(true);
                        StartingGame();
                        UE_LOG(LogTemp, Error, TEXT("Disabling !!!"));
                    }, 1, false);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameMode::Tick(float DeltaSeconds) -> GameStateVar is not Valid !!!"));
            }
        }
    }
}

void AFPS_MultiplayGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
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

void AFPS_MultiplayGameMode::UpdateGlobalGameTimer(int& min, int& sec)
{
    AFPS_MultiplayGameState* GameStateVar = GetGameState<AFPS_MultiplayGameState>();
    if (IsValid(GameStateVar))
    {
        if (--sec <= 0)
        {
            sec = 59;
            if (--min == -1)
            {
                if (GEngine)
                    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("GAME ENDED !!!")));

                min = 0;
                sec = 0;
                GetWorldTimerManager().ClearTimer(GameTimer);
                GameStateVar->GameEnded();
            }
        }
        GameStateVar->UpdateGameTime(min, sec);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameMode::UpdateGlobalGameTimer() -> GameStateVar is not Valid!!!"));
    }
}

void AFPS_MultiplayGameMode::UpdateObjectiveStats()
{
    AFPS_MultiplayGameState* GameStateVar = GetGameState<AFPS_MultiplayGameState>();
    if (IsValid(GameStateVar))
    {
        GameStateVar->UpdateObjectiveStats();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameMode::UpdateObjectiveStats() -> GameStateVar is not Valid!!!"));
    }
}

void AFPS_MultiplayGameMode::HandleStartingNewPlayer_Implementation(APlayerController* MovieSceneBlends)
{
    Super::HandleStartingNewPlayer_Implementation(MovieSceneBlends);
    UE_LOG(LogTemp, Warning, TEXT("%s HandleStartingNewPlayer -> Called !!!"), *MovieSceneBlends->GetName());

    AFPS_MultiplayPlayerController* PlayerController = Cast<AFPS_MultiplayPlayerController>(MovieSceneBlends);
    if (IsValid(PlayerController))
    {
        PlayerController->Team = assignTeam();
        FString PCTeam = PlayerController->Team == TEAM_BLUE ? "BLUE" : PlayerController->Team == TEAM_RED ? "RED" : "NONE";
        UE_LOG(LogTemp, Error, TEXT("%s PlayerController->Team = %s !!!"), *MovieSceneBlends->GetName(), *PCTeam);

        AFPS_MultiplayPlayerState* PlayerStateVar = MovieSceneBlends->GetPlayerState<AFPS_MultiplayPlayerState>();
        if (IsValid(PlayerStateVar))
        {
            PlayerStateVar->team = PlayerController->Team;
            FString PSVTeam = PlayerStateVar->team == TEAM_BLUE ? "BLUE" : PlayerStateVar->team == TEAM_RED ? "RED" : "NONE";
            UE_LOG(LogTemp, Error, TEXT("%s PlayerStateVar->Team = %s !!!"), *MovieSceneBlends->GetName(), *PSVTeam);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameMode::HandleStartingNewPlayer_Implementation(APlayerController* MovieSceneBlends) -> PlayerStateVar is not Valid !!!"), *MovieSceneBlends->GetName());
        }
        //SpawnDefaultPawnAtTransform(MovieSceneBlends, ChoosePlayerStart(PlayerController)->GetTransform());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameMode::HandleStartingNewPlayer_Implementation(APlayerController* MovieSceneBlends) -> PlayerController is not Valid !!!"), *MovieSceneBlends->GetName());
    }
}

void AFPS_MultiplayGameMode::PostLogin(APlayerController* MovieSceneBlends)
{
    Super::PostLogin(MovieSceneBlends);
}

void AFPS_MultiplayGameMode::StartingGame()
{
    AFPS_MultiplayGameState* GameStateVar = GetGameState<AFPS_MultiplayGameState>();
    if (IsValid(GameStateVar))
    {
        GetWorldTimerManager().ClearTimer(GameTimer);
        GetWorldTimerManager().SetTimer(StartingTimer, [this, GameStateVar]()
            {
                GameStateVar->DisablePlayersControls(false);
                GameStateVar->RespawnPlayers(true);
                GetWorldTimerManager().SetTimer(GameTimer, [this]()
                    {
                        UpdateGlobalGameTimer(minutes, seconds);
                    }, 1, true, 0.f);

            }, 4, false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayGameMode::StartingGame() -> GameStateVar is not Valid!!!"));
    }
}

// PreLogin -> ChoosePlayerStart -> Login -> PostLogin ???
// BeginPlay -> PostLogin

AActor* AFPS_MultiplayGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends)
{
    Super::ChoosePlayerStart_Implementation(MovieSceneBlends);
    UE_LOG(LogTemp, Warning, TEXT("%s ChoosePlayerStart -> Called !"), *MovieSceneBlends->GetName());

    if (IsValid(MovieSceneBlends))
    {
        AFPS_MultiplayPlayerState* PlayerStateVariable = MovieSceneBlends->GetPlayerState<AFPS_MultiplayPlayerState>();
        if (IsValid(PlayerStateVariable))
        {
            FString ThePlayerStateTeamName = PlayerStateVariable->team == TEAM_BLUE ? "BLUE" : PlayerStateVariable->team == TEAM_RED ? "RED" : "NONE";
            UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> PlayerStateVariable->Team = %s !"), *MovieSceneBlends->GetName(), *ThePlayerStateTeamName);

            TArray<AFPS_PlayerStart*> Starts;
            for (TActorIterator<AFPS_PlayerStart> StartItr(GetWorld()); StartItr; ++StartItr)
            {
                FString ThePlayerStartTeamName = StartItr->Team == TEAM_BLUE ? "BLUE" : StartItr->Team == TEAM_RED ? "RED" : "NONE";

                UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> Spawn->Team = %s !"), *MovieSceneBlends->GetName(), *ThePlayerStartTeamName);
                if (StartItr->Team == PlayerStateVariable->team)
                {
                    Starts.Add(*StartItr);
                }
            }
            if (Starts.Num() > 0)
            {
                AFPS_PlayerStart* TheStart = Starts[FMath::RandRange(0, Starts.Num() - 1)];
                FString StartTeamName = TheStart->Team == TEAM_BLUE ? "BLUE" : TheStart->Team == TEAM_RED ? "RED" : "NONE";
                UE_LOG(LogTemp, Warning, TEXT("Start Found = %s    %s!"), *TheStart->GetName(), *StartTeamName);
                return TheStart;
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> (Starts.Num() > 0) -> There are no Starts found !!!"), *MovieSceneBlends->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> PlayerStateVariable is not Valid !!!"), *MovieSceneBlends->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> MovieSceneBlends is not Valid !!!"), *MovieSceneBlends->GetName());
    }
    UE_LOG(LogTemp, Warning, TEXT("No START"));

    return nullptr;
}

TEnumAsByte<EFPS_Teams> AFPS_MultiplayGameMode::assignTeam()
{
    if (bluePlayers > redPlayers)
    {
        ++redPlayers;
        return TEAM_RED;
    }
    else
    {
        ++bluePlayers;
        return TEAM_BLUE;
    }
}



