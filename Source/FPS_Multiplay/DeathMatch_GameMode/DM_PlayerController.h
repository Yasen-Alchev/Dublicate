// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DM_PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "DM_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ADM_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UFUNCTION(Client, Reliable)
		void ClientRPCEndGame(const FString& WinnerName);

	UFUNCTION(Client, Reliable)
		void UpdateObjectiveStats(ADM_PlayerState* Player1, ADM_PlayerState* Player2, ADM_PlayerState* Player3);

	UFUNCTION(Client, Reliable)
		void ClientRPCUpdateGameTime(int minutes, int seconds);

private:
	FTimerHandle AntiBlurHandle;
};
