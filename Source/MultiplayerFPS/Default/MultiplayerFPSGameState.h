#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiplayerFPSGameState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMultiplayerFPSGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (ClampMin = "0", ClampMax = "30"))
		int PlayerRespawnTime;

	UFUNCTION()
		virtual void UpdateObjectiveStats();

	UFUNCTION()
		virtual void ResetStats();

	UFUNCTION()
		virtual void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		virtual void SetGlobalGameMessage(const FString& Message);

	UFUNCTION()
		virtual void ClearGlobalGameMessage();

	UFUNCTION()
		virtual void GameEnded();

	UFUNCTION()
		virtual void RespawnPlayers(bool instant = false);

	UFUNCTION()
		virtual void KillPlayers();

	UFUNCTION()
		virtual void DisablePlayersControls(bool bDisable);

};


