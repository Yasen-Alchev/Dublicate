#pragma once

#include "CoreMinimal.h"
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

	UFUNCTION()
		virtual int getPlayerKills() const { return this->Kills; }

	UFUNCTION()
		virtual int getPlayerDeaths() const { return this->Deaths; }

	UFUNCTION()
		virtual int getPlayerScore() const { return this->PlayerScore; }

	UFUNCTION(Server, Reliable)
		virtual void ServerHasKilledPlayer();

	UFUNCTION(Server, Reliable)
		virtual void ServerHasDied();

protected:

	UPROPERTY(Replicated)
		int32 Kills;

	UPROPERTY(Replicated)
		int32 Deaths;

	UPROPERTY(Replicated)
		int32 PlayerScore;
};

