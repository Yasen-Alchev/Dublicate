// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSCharacter.h"
#include "DM_Character.generated.h"

UCLASS(config = Game)
class MULTIPLAYERFPS_API ADM_Character : public AMultiplayerFPSCharacter
{
	GENERATED_BODY()

public:

	ADM_Character();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
