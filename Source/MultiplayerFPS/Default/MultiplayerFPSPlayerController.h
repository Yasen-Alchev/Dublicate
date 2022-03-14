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

	UFUNCTION(Client, Reliable)
		virtual void ClientEndGame(const FString& Winner);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateGameTime(int minutes, int seconds);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateObjectiveStats(const TArray<FString>& ObjectiveStats);

	UFUNCTION(Server, Reliable)
		virtual void ServerSpawnPlayer();

	UFUNCTION()
		virtual void RespawnPlayer(bool instant = false);

	UFUNCTION(Server, Reliable)
		virtual void ServerRespawnPlayer();

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

