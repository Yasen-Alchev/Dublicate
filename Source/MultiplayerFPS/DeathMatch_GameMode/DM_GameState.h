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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxFlagsToCapture", meta = (ClampMin = "1", UIMin = "1", UIMax = "10"))
		int MaxFlagsToCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (ClampMin = "0", ClampMax = "30"))
		int PlayerRespawnTime;

	UFUNCTION()
		void UpdateLeaders();

	UFUNCTION()
		void UpdateObjectiveStats();

	UFUNCTION()
		void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		void GameEnded();

	UFUNCTION()
		virtual void RespawnPlayers(bool instant = false);

	UFUNCTION()
		virtual void KillPlayers();

	UFUNCTION()
		virtual void DisablePlayersControls(bool bDisable);

private:
	UPROPERTY()
	ADM_PlayerState* Player1;

	UPROPERTY()
	ADM_PlayerState* Player2;

	UPROPERTY()
	ADM_PlayerState* Player3;
};
