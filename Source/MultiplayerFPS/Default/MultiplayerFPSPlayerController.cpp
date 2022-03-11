#include "MultiplayerFPSPlayerController.h"

#include "MultiplayerFPSTeamBasedCharacter.h"
#include "MultiplayerFPSGameMode.h"
#include "MultiplayerFPSGameState.h"
#include "MultiplayerFPSInGameHUD.h"

AMultiplayerFPSPlayerController::AMultiplayerFPSPlayerController(){}

void AMultiplayerFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	ServerSpawnPlayer();
}

void AMultiplayerFPSPlayerController::OnPossess(APawn* MovieSceneBlends)
{
	Super::OnPossess(MovieSceneBlends);

	AMultiplayerFPSTeamBasedCharacter* MyPawn = Cast<AMultiplayerFPSTeamBasedCharacter>(MovieSceneBlends);
	if (IsValid(MyPawn))
	{
		MyPawn->InitTeam();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AMultiplayerFPSPlayerController::OnPossess(APawn* MovieSceneBlends) -> MyPawn is not Valid !!!"), *this->GetName());
	}
}

void AMultiplayerFPSPlayerController::ClientEndGame_Implementation(ETeams WinnerTeam)
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
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
			InGameHud->GameEnded(WinnerTeam);
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

void AMultiplayerFPSPlayerController::ClientUpdateObjectiveStats_Implementation(int32 RedScore, int32 BlueScore)
{
	AMultiplayerFPSInGameHUD* InGameHud = Cast<AMultiplayerFPSInGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->UpdateObjectiveStats(RedScore, BlueScore);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::UpdateObjectiveStats_Implementation(int32 RedScore, int32 BlueScore) -> InGameHud is not Valid !!!"));
	}
}

void AMultiplayerFPSPlayerController::ServerSpawnPlayer_Implementation()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			AMultiplayerFPSGameMode* GameMode = Cast<AMultiplayerFPSGameMode>(GetWorld()->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				GameMode->RestartPlayerAtPlayerStart(this, GameMode->ChoosePlayerStart(this));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerSpawnPlayer_Implementation() -> GameMode is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerSpawnPlayer_Implementation() -> World is  not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSPlayerController::ServerSpawnPlayer_Implementation() -> Does not have Authority !!!"));
	}
}

void AMultiplayerFPSPlayerController::RespawnPlayer(bool instant)
{
	KillPlayer();
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
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			AMultiplayerFPSGameMode* GameMode = Cast<AMultiplayerFPSGameMode>(World->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				APawn* NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
				AMultiplayerFPSTeamBasedCharacter* NewPlayerPawn = Cast<AMultiplayerFPSTeamBasedCharacter>(NewPawn);
				if (NewPlayerPawn)
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
	AMultiplayerFPSTeamBasedCharacter* PlayerPawn = Cast<AMultiplayerFPSTeamBasedCharacter>(GetPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->Destroy(true);
		UnPossess();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s AMultiplayerFPSPlayerController::KillPlayer() -> PlayerPawn is not Valid !!!"), *this->GetName());
	}
}

