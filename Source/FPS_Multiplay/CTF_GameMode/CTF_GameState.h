// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayGameState.h"
#include "GameFramework/GameState.h"
#include "CTF_GameState.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API ACTF_GameState : public AFPS_MultiplayGameState
{
	GENERATED_BODY()

public:
	ACTF_GameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (ClampMin = "0", ClampMax = "40"))
	int FlagRespawnTime;


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};


