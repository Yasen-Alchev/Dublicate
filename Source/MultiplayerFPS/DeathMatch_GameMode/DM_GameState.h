#pragma once

#include "CoreMinimal.h"
#include "DM_PlayerState.h"
#include "GameFramework/GameState.h"
#include "DM_GameState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ADM_GameState : public AGameState
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
	UPROPERTY()
	ADM_PlayerState* Player1;

	UPROPERTY()
	ADM_PlayerState* Player2;

	UPROPERTY()
	ADM_PlayerState* Player3;
};
