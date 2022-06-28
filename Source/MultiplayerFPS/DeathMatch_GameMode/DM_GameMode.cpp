#include "DM_GameMode.h"

#include "DM_GameState.h"
#include "DM_InGameHUD.h"
#include "DM_PlayerController.h"
#include "DM_PlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerFPS/CommonClasses/PlayerStartPoint.h"

ADM_GameMode::ADM_GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	HUDClass = ADM_InGameHUD::StaticClass();
	PlayerControllerClass = ADM_PlayerController::StaticClass();
	PlayerStateClass = ADM_PlayerState::StaticClass();

	minutes = 5;
	seconds = 0;
	minPlayersToStart = 2;
	bStarted = false;
}

void ADM_GameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
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

void ADM_GameMode::BeginPlay()
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


void ADM_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ADM_GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ADM_GameMode::UpdateGlobalGameTimer(int& min, int& sec)
{
	ADM_GameState* GameStateVar = GetGameState<ADM_GameState>();
	if (IsValid(GameStateVar))
	{
		if (--sec <= 0)
		{
			sec = 59;
			if (--min == -1)
			{
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
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameMode::UpdateGlobalGameTimer() -> GameStateVar is not Valid!!!"));
	}
}

void ADM_GameMode::UpdateObjectiveStats()
{
	ADM_GameState* GameStateVar = GetGameState<ADM_GameState>();
	if (IsValid(GameStateVar))
	{
		GameStateVar->UpdateObjectiveStats();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_GameMode::UpdateObjectiveStats() -> GameStateVar is not Valid!!!"));
	}
}

void ADM_GameMode::StartingGame()
{
	ADM_GameState* GameStateVar = GetGameState<ADM_GameState>();
	if (IsValid(GameStateVar))
	{
		GetWorldTimerManager().ClearTimer(GameTimer);
		GetWorldTimerManager().SetTimer(StartingTimer, [this, GameStateVar]()
			{
				GameStateVar->KillPlayers();
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
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSGameMode::StartingGame() -> GameStateVar is not Valid!!!"));
	}
}

AActor* ADM_GameMode::ChoosePlayerStart_Implementation(AController* MovieSceneBlends)
{
	Super::ChoosePlayerStart_Implementation(MovieSceneBlends);

	if (IsValid(MovieSceneBlends))
	{

		ADM_PlayerState* PlayerStateVariable = MovieSceneBlends->GetPlayerState<ADM_PlayerState>();
		if (IsValid(PlayerStateVariable))
		{
			TArray<AActor*> Starts;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
			return Starts[FMath::RandRange(0, Starts.Num() - 1)];
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

