// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSGameState.h"
#include "GameFramework/GameState.h"
#include "CTF_GameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API ACTF_GameState : public AMultiplayerFPSGameState
{
	GENERATED_BODY()

public:
	ACTF_GameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (ClampMin = "0", ClampMax = "40"))
	int FlagRespawnTime;


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};


