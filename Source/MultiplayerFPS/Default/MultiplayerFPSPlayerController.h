// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/CommonClasses/Teams.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerFPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Client, Reliable)
		virtual void ClientEndGame(ETeams WinnerTeam);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateGameTime(int minutes, int seconds);

	UFUNCTION(Client, Reliable)
		virtual void ClientUpdateObjectiveStats(int32 RedScore, int32 BlueScore);

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

	UPROPERTY()
		TEnumAsByte<ETeams> Team;

protected:

	UPROPERTY()
		FTimerHandle AntiBlurHandle;

	UPROPERTY()
		FTimerHandle RespawnHandle;
};

