// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DM_PlayerState.h"
#include "GameFramework/GameState.h"
#include "DM_GameState.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ADM_GameState : public AGameState
{
	GENERATED_BODY()

public:
	ADM_GameState();

	virtual void BeginPlay() override;

	UFUNCTION()
		void UpdateLeaders();

	UFUNCTION()
		void UpdateObjectiveStats();

	UFUNCTION()
		void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		void GameEnded();

private:
	ADM_PlayerState* Player1;
	ADM_PlayerState* Player2;
	ADM_PlayerState* Player3;
};
