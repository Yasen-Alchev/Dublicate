// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayCharacter.h"
#include "DM_Character.generated.h"

UCLASS(config = Game)
class FPS_MULTIPLAY_API ADM_Character : public AFPS_MultiplayCharacter
{
	GENERATED_BODY()

public:

	ADM_Character();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
