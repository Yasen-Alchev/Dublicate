// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayPlayerController.h"

#include "FPS_MultiplayTeamBasedCharacter.h"
#include "FPS_MultiplayGameMode.h"
#include "FPS_MultiplayGameState.h"
#include "FPS_MultiplayInGameHUD.h"


void AFPS_MultiplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Error, TEXT("%s BeginPlay Controller"), *this->GetName());

	SetInputMode(FInputModeGameOnly());
	ServerSpawnPlayer();
}

void AFPS_MultiplayPlayerController::OnPossess(APawn* MovieSceneBlends)
{
	Super::OnPossess(MovieSceneBlends);

	UE_LOG(LogTemp, Warning, TEXT("%s OnPossess Controller Called"), *this->GetName());

	AFPS_MultiplayTeamBasedCharacter* MyPawn = Cast<AFPS_MultiplayTeamBasedCharacter>(MovieSceneBlends);
	if (IsValid(MyPawn))
	{
		MyPawn->InitTeam();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AFPS_MultiplayPlayerController::OnPossess(APawn* MovieSceneBlends) -> MyPawn is not Valid !!!"), *this->GetName());
	}
}

void AFPS_MultiplayPlayerController::ClientEndGame_Implementation(EFPS_Teams WinnerTeam)
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

		AFPS_MultiplayInGameHUD* InGameHud = Cast<AFPS_MultiplayInGameHUD>(GetHUD());
		if (IsValid(InGameHud))
		{
			SetShowMouseCursor(true);
			InGameHud->GameEnded(WinnerTeam);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerController::ClientRPCEndGame_Implementation(EFPS_Teams WinnerTeam) -> InGameHud is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerController::ClientRPCEndGame_Implementation(EFPS_Teams WinnerTeam) -> World is not Valid !!!"));
	}
}

void AFPS_MultiplayPlayerController::ClientUpdateGameTime_Implementation(int minutes, int seconds)
{
	AFPS_MultiplayInGameHUD* InGameHud = Cast<AFPS_MultiplayInGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->UpdateGameTime(minutes, seconds);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerController::ClientRPCUpdateGameTime_Implementation(int minutes, int seconds) -> InGameHud is not Valid !!!"));
	}
}

void AFPS_MultiplayPlayerController::ClientUpdateObjectiveStats_Implementation(int32 RedScore, int32 BlueScore)
{
	AFPS_MultiplayInGameHUD* InGameHud = Cast<AFPS_MultiplayInGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->UpdateObjectiveStats(RedScore, BlueScore);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerController::UpdateObjectiveStats_Implementation(int32 RedScore, int32 BlueScore) -> InGameHud is not Valid !!!"));
	}
}

void AFPS_MultiplayPlayerController::ServerSpawnPlayer_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerController::ServerSpawnPlayer_Implementation() -> Called !!!"));
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			AFPS_MultiplayGameMode* GameMode = Cast<AFPS_MultiplayGameMode>(GetWorld()->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				GameMode->RestartPlayerAtPlayerStart(this, GameMode->ChoosePlayerStart(this));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerController::ServerSpawnPlayer_Implementation() -> GameMode is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerController::ServerSpawnPlayer_Implementation() -> World is  not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayPlayerController::ServerSpawnPlayer_Implementation() -> Does not have Authority !!!"));
	}
}

void AFPS_MultiplayPlayerController::RespawnPlayer(bool instant)
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
			AFPS_MultiplayGameState* GameState = Cast<AFPS_MultiplayGameState>(World->GetGameState());
			if (IsValid(GameState))
			{
				GetWorldTimerManager().SetTimer(RespawnHandle, this, &AFPS_MultiplayPlayerController::ServerRespawnPlayer, GameState->PlayerRespawnTime);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayPlayerController::RespawnPlayer(bool instant) -> GameState is not Valid !!!"), *this->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayPlayerController::RespawnPlayer(bool instant) -> World is not Valid !!!"), *this->GetName());
		}
	}
}

void AFPS_MultiplayPlayerController::ServerRespawnPlayer_Implementation()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			AFPS_MultiplayGameMode* GameMode = Cast<AFPS_MultiplayGameMode>(World->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				APawn* NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
				AFPS_MultiplayTeamBasedCharacter* NewPlayerPawn = Cast<AFPS_MultiplayTeamBasedCharacter>(NewPawn);
				if (NewPlayerPawn)
				{
					Possess(NewPlayerPawn);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("%s AFPS_MultiplayPlayerController::ServerRPCRespawnPlayer_Implementation() -> NewPlayerPawn is not Valid !!!"), *this->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s AFPS_MultiplayPlayerController::ServerRPCRespawnPlayer_Implementation() -> GameMode is not Valid !!!"), *this->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s AFPS_MultiplayPlayerController::ServerRPCRespawnPlayer_Implementation() -> World is not Valid !!!"), *this->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s AFPS_MultiplayPlayerController::ServerRPCRespawnPlayer_Implementation() -> Does not have Authority !!!"), *this->GetName());
	}
}

void AFPS_MultiplayPlayerController::DisableControls_Implementation(bool disable)
{
	SetIgnoreMoveInput(disable);
	SetIgnoreLookInput(disable);
}

void AFPS_MultiplayPlayerController::KillPlayer()
{
	AFPS_MultiplayTeamBasedCharacter* PlayerPawn = Cast<AFPS_MultiplayTeamBasedCharacter>(GetPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->Destroy(true);
		UnPossess();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s AFPS_MultiplayPlayerController::KillPlayer() -> PlayerPawn is not Valid !!!"), *this->GetName());
	}
}

