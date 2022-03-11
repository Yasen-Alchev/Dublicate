#include "DM_PlayerController.h"

#include "DM_Character.h"
#include "DM_GameMode.h"
#include "DM_GameState.h"
#include "DM_InGameHUD.h"
#include "Kismet/KismetSystemLibrary.h"

void ADM_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	ServerSpawnPlayer();
}

void ADM_PlayerController::ClientRPCEndGame_Implementation(const FString& WinnerName)
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

		ADM_InGameHUD* InGameHud = Cast<ADM_InGameHUD>(GetHUD());
		if (IsValid(InGameHud))
		{
			SetShowMouseCursor(true);
			InGameHud->GameEnded(WinnerName);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_PlayerController::ClientRPCEndGame_Implementation(FName WinnerName) -> InGameHud is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_PlayerController::ClientRPCEndGame_Implementation(FName WinnerName) -> World is not Valid !!!"));
	}
}

void ADM_PlayerController::ClientRPCUpdateGameTime_Implementation(int minutes, int seconds)
{
	ADM_InGameHUD* InGameHud = Cast<ADM_InGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->UpdateGameTime(minutes, seconds);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_PlayerController::ClientRPCUpdateGameTime_Implementation(int minutes, int seconds) -> InGameHud is not Valid !!!"));
	}
}

void ADM_PlayerController::UpdateObjectiveStats_Implementation(ADM_PlayerState* Player1, ADM_PlayerState* Player2, ADM_PlayerState* Player3)
{
	ADM_InGameHUD* InGameHud = Cast<ADM_InGameHUD>(GetHUD());
	if (IsValid(InGameHud))
	{
		InGameHud->UpdateObjectiveStats(Player1, Player2, Player3);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_PlayerController::ClientRPCUpdateObjectiveStats() -> InGameHud is not Valid !!!"));
	}
}


void ADM_PlayerController::ServerSpawnPlayer_Implementation()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			ADM_GameMode* GameMode = Cast<ADM_GameMode>(GetWorld()->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				GameMode->RestartPlayerAtPlayerStart(this, GameMode->ChoosePlayerStart(this));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ADM_PlayerController::ServerSpawnPlayer_Implementation() -> GameMode is not Valid !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_PlayerController::ServerSpawnPlayer_Implementation() -> World is  not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_PlayerController::ServerSpawnPlayer_Implementation() -> Does not have Authority !!!"));
	}
}

void ADM_PlayerController::RespawnPlayer(bool instant)
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
			ADM_GameState* GameState = Cast<ADM_GameState>(World->GetGameState());
			if (IsValid(GameState))
			{
				GetWorldTimerManager().SetTimer(RespawnHandle, this, &ADM_PlayerController::ServerRespawnPlayer, GameState->PlayerRespawnTime);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("%s ADM_PlayerController::RespawnPlayer(bool instant) -> GameState is not Valid !!!"), *this->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s ADM_PlayerController::RespawnPlayer(bool instant) -> World is not Valid !!!"), *this->GetName());
		}
	}
}

void ADM_PlayerController::ServerRespawnPlayer_Implementation()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			ADM_GameMode* GameMode = Cast<ADM_GameMode>(World->GetAuthGameMode());
			if (IsValid(GameMode))
			{
				APawn* NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
				ADM_Character* NewPlayerPawn = Cast<ADM_Character>(NewPawn);
				if (NewPlayerPawn)
				{
					Possess(NewPlayerPawn);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("%s ADM_PlayerController::ServerRPCRespawnPlayer_Implementation() -> NewPlayerPawn is not Valid !!!"), *this->GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%s ADM_PlayerController::ServerRPCRespawnPlayer_Implementation() -> GameMode is not Valid !!!"), *this->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s ADM_PlayerController::ServerRPCRespawnPlayer_Implementation() -> World is not Valid !!!"), *this->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s ADM_PlayerController::ServerRPCRespawnPlayer_Implementation() -> Does not have Authority !!!"), *this->GetName());
	}
}

void ADM_PlayerController::DisableControls_Implementation(bool disable)
{
	SetIgnoreMoveInput(disable);
	SetIgnoreLookInput(disable);
}

void ADM_PlayerController::KillPlayer()
{
	ADM_Character* PlayerPawn = Cast<ADM_Character>(GetPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->Destroy(true);
		UnPossess();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s ADM_PlayerController::KillPlayer() -> PlayerPawn is not Valid !!!"), *this->GetName());
	}
}


