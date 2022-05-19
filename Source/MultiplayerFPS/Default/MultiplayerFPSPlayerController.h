#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPSPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerFPSPlayerController.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AMultiplayerFPSPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
		bool bShouldRespawn;

	UFUNCTION(Client, Reliable)
		virtual void ClientEndGame(const FString& Winner);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateGameTime(int minutes, int seconds);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateObjectiveStats(const TArray<FString>& ObjectiveStats);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateLeaderBoardStats();

	UFUNCTION(Client, Reliable)
		virtual void ClientSetGlobalGameMessage(const FString& Message);

	UFUNCTION(Client, Reliable)
		virtual void ClientClearGlobalGameMessage();

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

