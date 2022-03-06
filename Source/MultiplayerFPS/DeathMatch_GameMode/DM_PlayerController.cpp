// Fill out your copyright notice in the Description page of Project Settings.


#include "DM_PlayerController.h"
#include "DM_InGameHUD.h"
#include "Kismet/KismetSystemLibrary.h"

void ADM_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
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
		UE_LOG(LogTemp, Error, TEXT("ACTF_PlayerController::ClientRPCUpdateObjectiveStats() -> InGameHud is not Valid !!!"));
	}
}


