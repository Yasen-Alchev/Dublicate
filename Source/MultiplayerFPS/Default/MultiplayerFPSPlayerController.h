#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerFPSPlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AMultiplayerFPSPlayerController();

	virtual void BeginPlay() override;

	UPROPERTY()
		bool bShouldRespawn;

	UFUNCTION(Client, Reliable)
		virtual void ClientEndGame(const FString& Winner);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateGameTime(int minutes, int seconds);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateObjectiveStats(const TArray<FString>& ObjectiveStats);

	UFUNCTION(Server, Reliable)
		virtual void ServerRestartPlayerOnStart();

	UFUNCTION()
		virtual void RespawnPlayer(bool instant = false);

	UFUNCTION(Client, Reliable)
		virtual void DisableControls(bool disable);

	UFUNCTION()
		virtual void KillPlayer();

protected:

	UFUNCTION(Server, Reliable)
		virtual void ServerRespawnPlayer();

	UPROPERTY()
		FTimerHandle AntiBlurHandle;

	UPROPERTY()
		FTimerHandle RespawnHandle;
};

