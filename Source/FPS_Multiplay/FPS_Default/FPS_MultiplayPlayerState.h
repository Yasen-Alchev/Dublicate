// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_CommonClasses/FPS_Teams.h"
#include "GameFramework/PlayerState.h"
#include "FPS_MultiplayPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API AFPS_MultiplayPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AFPS_MultiplayPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Team")
		TEnumAsByte<EFPS_Teams> team;

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

