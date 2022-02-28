// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenu_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MULTIPLAY_API AMainMenu_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

};
