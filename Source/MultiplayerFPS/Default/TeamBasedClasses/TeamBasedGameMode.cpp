#include "TeamBasedGameMode.h"

#include "EngineUtils.h"
#include "TeamBasedPlayerController.h"
#include "TeamBasedPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/CommonClasses/PlayerStartPoint.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameMode.h"

ATeamBasedGameMode::ATeamBasedGameMode()
{
    RedPlayers = 0;
    BluePlayers = 0;
}

void ATeamBasedGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void ATeamBasedGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ATeamBasedGameMode::HandleStartingNewPlayer_Implementation(APlayerController* MovieSceneBlends)
{
    Super::HandleStartingNewPlayer_Implementation(MovieSceneBlends);

    ATeamBasedPlayerState* PlayerStateVar = MovieSceneBlends->GetPlayerState<ATeamBasedPlayerState>();
    if (!IsValid(PlayerStateVar))
    {
        UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedGameMode::HandleStartingNewPlayer_Implementation(APlayerController* MovieSceneBlends) -> PlayerStateVar is not Valid !!!"), *MovieSceneBlends->GetName());
        return;
    }
    PlayerStateVar->Team = AssignTeam();
}

AActor* ATeamBasedGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends)
{
    Super::ChoosePlayerStart_Implementation(MovieSceneBlends);

    if (!IsValid(MovieSceneBlends))
    {
        UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> MovieSceneBlends is not Valid !!!"), *MovieSceneBlends->GetName());
        return nullptr;
    }

	ATeamBasedPlayerState* PlayerStateVariable = MovieSceneBlends->GetPlayerState<ATeamBasedPlayerState>();
    if (!IsValid(PlayerStateVariable))
    {
        UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> PlayerStateVariable is not Valid !!!"), *MovieSceneBlends->GetName());
        return nullptr;
    }

	TArray<APlayerStartPoint*> Starts;
    for (TActorIterator<APlayerStartPoint> StartItr(GetWorld()); StartItr; ++StartItr)
    {
        if (StartItr->Team == PlayerStateVariable->Team)
        {
            Starts.Add(*StartItr);
        }
    }
    if (Starts.Num() > 0)
    {
        APlayerStartPoint* TheStart = Starts[FMath::RandRange(0, Starts.Num() - 1)];
        return TheStart;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s ATeamBasedGameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends) -> (Starts.Num() > 0) -> There are no Starts found !!!"), *MovieSceneBlends->GetName());
    	return nullptr;
    }
}

TEnumAsByte<ETeams> ATeamBasedGameMode::AssignTeam()
{
    if (BluePlayers > RedPlayers)
    {
        ++RedPlayers;
        return TEAM_RED;
    }
    else
    {
        ++BluePlayers;
        return TEAM_BLUE;
    }
}



