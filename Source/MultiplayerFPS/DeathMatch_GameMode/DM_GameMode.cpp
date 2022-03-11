#include "DM_GameMode.h"

#include "DM_GameState.h"
#include "DM_InGameHUD.h"
#include "DM_PlayerController.h"
#include "DM_PlayerState.h"
#include "Blueprint/UserWidget.h"

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

	GetWorldTimerManager().SetTimer(GameTimer, this, &ADM_GameMode::UpdateGlobalGameTimer, 1, true, 0.f);
}

void ADM_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ADM_GameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ADM_GameMode::UpdateGlobalGameTimer()
{
	ADM_GameState* GameStateVar = GetGameState<ADM_GameState>();
	if (IsValid(GameStateVar))
	{
		if (--seconds <= 0)
		{
			seconds = 59;
			if (--minutes == -1)
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("GAME ENDED !!!")));

				minutes = 0;
				seconds = 0;
				GetWorldTimerManager().ClearTimer(GameTimer);
				GameStateVar->GameEnded();
			}
		}
		GameStateVar->UpdateGameTime(minutes, seconds);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_GameMode::UpdateGlobalGameTimer() -> GameStateVar is not Valid!!!"));
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
