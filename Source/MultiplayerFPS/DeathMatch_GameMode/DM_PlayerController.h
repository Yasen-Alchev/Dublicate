#pragma once

#include "CoreMinimal.h"
#include "DM_PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "DM_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ADM_PlayerController : public APlayerController
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

	UFUNCTION(Server, Reliable)
		virtual void ServerSpawnPlayer();

	UFUNCTION(Server, Reliable)
		virtual void ServerRespawnPlayer();

	UFUNCTION()
		virtual void RespawnPlayer(bool instant = false);

	UFUNCTION(Client, Reliable)
		virtual void DisableControls(bool disable);

	UFUNCTION()
		virtual void KillPlayer();

protected:

	UPROPERTY()
		FTimerHandle AntiBlurHandle;

	UPROPERTY()
		FTimerHandle RespawnHandle;
};