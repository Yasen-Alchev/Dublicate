#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiplayerFPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSGameState : public AGameState
{
	GENERATED_BODY()

public:
	AMultiplayerFPSGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxFlagsToCapture", meta = (ClampMin = "1", UIMin = "1", UIMax = "10"))
		int MaxFlagsToCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (ClampMin = "0", ClampMax = "30"))
		int PlayerRespawnTime;

	UFUNCTION()
		virtual void RedFlagCaptured();

	UFUNCTION()
		virtual void BlueFlagCaptured();

	UFUNCTION()
		virtual int GetRedTeamCapturedFlags() { return RedTeamCapturedFlags; }

	UFUNCTION()
		virtual int GetBlueTeamCapturedFlags() { return BlueTeamCapturedFlags; }

	UFUNCTION()
		virtual void UpdateObjectiveStats();

	UFUNCTION()
		virtual void ResetStats();

	UFUNCTION()
		virtual void UpdateGameTime(int minutes, int seconds);

	UFUNCTION()
		virtual void GameEnded();

	UFUNCTION()
		virtual void RespawnPlayers(bool instant = false);

	UFUNCTION()
		virtual void KillPlayers();

	UFUNCTION()
		virtual void DisablePlayersControls(bool bDisable);

protected:

	UPROPERTY(Replicated)
		int RedTeamCapturedFlags;

	UPROPERTY(Replicated)
		int BlueTeamCapturedFlags;
};


