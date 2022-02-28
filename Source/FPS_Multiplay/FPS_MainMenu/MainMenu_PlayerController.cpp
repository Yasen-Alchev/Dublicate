// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu_PlayerController.h"

void AMainMenu_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock));
	SetShowMouseCursor(true);
}
