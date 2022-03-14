#include "DM_GameMode.h"

#include "DM_GameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ADM_GameMode::ADM_GameMode() {}

void ADM_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void  ADM_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bStarted)
	{
		if (NumPlayers >= minPlayersToStart)
		{
			ADM_GameState* GameStateVar = GetGameState<ADM_GameState>();
			if (IsValid(GameStateVar))
			{
				bStarted = true;
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

