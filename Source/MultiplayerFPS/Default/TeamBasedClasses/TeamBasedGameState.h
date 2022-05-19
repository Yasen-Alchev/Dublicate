#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameState.h"
#include "TeamBasedGameState.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ATeamBasedGameState : public AMultiplayerFPSGameState
{
	GENERATED_BODY()

public:
	ATeamBasedGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxFlagsToCapture", meta = (ClampMin = "1", UIMin = "1", UIMax = "10"))
		int MaxFlagsToCapture;

	UFUNCTION()
		virtual void RedFlagCaptured();

	UFUNCTION()
		virtual void BlueFlagCaptured();

	UFUNCTION()
		virtual int GetRedTeamCapturedFlags() { return RedTeamCapturedFlags; }

	UFUNCTION()
		virtual int GetBlueTeamCapturedFlags() { return BlueTeamCapturedFlags; }

	virtual void UpdateObjectiveStats() override;

	virtual void ResetStats() override;

	virtual void GameEnded() override;

	UFUNCTION()
		virtual  void UpdateLeaderBoards();

protected:

	UPROPERTY(Replicated)
		int RedTeamCapturedFlags;

	UPROPERTY(Replicated)
		int BlueTeamCapturedFlags;
};


