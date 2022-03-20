#include "MultiplayerFPSPlayerController.h"

#include "MultiplayerFPSGameMode.h"
#include "MultiplayerFPSGameState.h"
#include "MultiplayerFPSInGameHUD.h"
#include "TeamBasedClasses/TeamBasedCharacter.h"

AMultiplayerFPSPlayerController::AMultiplayerFPSPlayerController()
{
	bShouldRespawn = true;
}

void AMultiplayerFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	ServerRestartPlayerOnStart();
}

void AMultiplayerFPSPlayerController::ClientEndGame_Implementation(const FString& Winner)
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		bShouldRespawn = false;
		SetIgnoreMoveInput(true);
		SetIgnoreLookInput(true);
		World->GetTimerManager().SetTimer(AntiBlurHandle, [this, World]()
			{
				World->GetTimerManager().ClearTimer(AntiBlurHandle);
				SetPause(true);
			}, 0.5f, false);

		AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
		if (IsValid(InGameHud))
		{
			SetShowMouseCursor(true);
			InGameHud->GameEnded(Winner);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientRPCEndGame_Implementation(ETeams WinnerTeam) -> InGameHud is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientRPCEndGame_Implementation(ETeams WinnerTeam) -> World is not Valid !!!"));
	}
}

void AMultiplayerFPSPlayerController::ClientUpdateGameTime_Implementation(int minutes, int seconds)
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->UpdateGameTime(minutes, seconds);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientRPCUpdateGameTime_Implementation(int minutes, int seconds) -> InGameHud is not Valid !!!"));
	}
}

void AMultiplayerFPSPlayerController::ClientUpdateObjectiveStats_Implementation(const TArray<FString>& ObjectiveStats)
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->UpdateObjectiveStats(ObjectiveStats);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::UpdateObjectiveStats_Implementation(int32 RedScore, int32 BlueScore) -> InGameHud is not Valid !!!"));
	}
}

void AMultiplayerFPSPlayerController::ServerRestartPlayerOnStart_Implementation()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			AMultiplayerFPSGameMode* GameMode = Cast<AMultiplayerFPSGameMode>(World->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				GameMode->RestartPlayerAtPlayerStart(this, GameMode->ChoosePlayerStart(this));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerRestartPlayerOnStart_Implementation() -> GameMode is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerRestartPlayerOnStart_Implementation() -> World is  not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerRestartPlayerOnStart_Implementation() -> Does not have Authority !!!"));
	}
}

void AMultiplayerFPSPlayerController::ClientSetGlobalGameMessage_Implementation(const FString& Message)
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->SetGlobalGameMessage(Message);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientSetGlobalGameMessage_Implementation(FString Message) -> InGameHud is not Valid !!!"));
	}
}

void AMultiplayerFPSPlayerController::ClientClearGlobalGameMessage_Implementation()
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->ClearGlobalGameMessage();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ClientClearGlobalGameMessage_Implementation() -> InGameHud is not Valid !!!"));
	}
}

void AMultiplayerFPSPlayerController::RespawnPlayer(bool instant)
{
	KillPlayer();
	if (!bShouldRespawn) return;

	if (instant)
	{
		ServerRespawnPlayer();
	}
	else
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			AMultiplayerFPSGameState* GameState = Cast<AMultiplayerFPSGameState>(World->GetGameState());
			if (IsValid(GameState))
			{
				GetWorldTimerManager().SetTimer(RespawnHandle, this, &AMultiplayerFPSPlayerController::ServerRespawnPlayer, GameState->PlayerRespawnTime);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSPlayerController::RespawnPlayer(bool instant) -> GameState is not Valid !!!"), *this->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSPlayerController::RespawnPlayer(bool instant) -> World is not Valid !!!"), *this->GetName());
		}
	}
}

void AMultiplayerFPSPlayerController::ServerRespawnPlayer_Implementation()
{
	if (!bShouldRespawn) return;

	GetWorldTimerManager().ClearTimer(RespawnHandle);
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			AMultiplayerFPSGameMode* GameMode = Cast<AMultiplayerFPSGameMode>(World->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				APawn* NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
				if (IsValid(NewPawn))
				{
					AMultiplayerFPSCharacter* NewPlayerPawn = Cast<AMultiplayerFPSCharacter>(NewPawn);
					if (IsValid(NewPlayerPawn))
					{
						Possess(NewPlayerPawn);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> NewPlayerPawn is not Valid !!!"), *this->GetName());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> NewPawn is not Valid !!!"), *this->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> GameMode is not Valid !!!"), *this->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> World is not Valid !!!"), *this->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::ServerRPCRespawnPlayer_Implementation() -> Does not have Authority !!!"), *this->GetName());
	}
}

void AMultiplayerFPSPlayerController::DisableControls_Implementation(bool disable)
{
	SetIgnoreMoveInput(disable);
	SetIgnoreLookInput(disable);
}

void AMultiplayerFPSPlayerController::KillPlayer()
{
	AMultiplayerFPSCharacter* PlayerPawn = Cast<AMultiplayerFPSCharacter>(GetPawn());
	if (IsValid(PlayerPawn))
	{
		if (HasAuthority())
		{
			PlayerPawn->bDead = true;
			PlayerPawn->ClientDestoryPlayer();
		}
		PlayerPawn->DestoryPlayer();
		UnPossess();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSPlayerController::ServerKillPlayer() -> PlayerPawn is not Valid !!!"), *this->GetName());
	}
}

