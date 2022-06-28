#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerFPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AMultiplayerFPSPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Team")
		TEnumAsByte<ETeams> team;

	UFUNCTION()
		virtual int getCapturedFlagsCount() const { return FlagsCaptured; }

	UFUNCTION()
		virtual int getPlayerKills() const { return Kills; }

	UFUNCTION()
		virtual int getPlayerDeaths() const { return Deaths; }

	UFUNCTION()
		virtual int getPlayerScore() const { return PlayerScore; }

	UFUNCTION()
		virtual void FlagWasCaptured();

protected:
	UPROPERTY(Replicated)
		int FlagsCaptured;

	UPROPERTY(Replicated)
		int Kills;

	UPROPERTY(Replicated)
		int Deaths;

	UPROPERTY(Replicated)
		int PlayerScore;
};

