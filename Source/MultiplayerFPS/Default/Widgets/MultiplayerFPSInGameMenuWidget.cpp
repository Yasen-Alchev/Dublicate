// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSInGameMenuWidget.h"
#include "MultiplayerFPS/DeathMatch_GameMode/DM_Character.h"
#include "Runtime/UMG/Public/UMG.h"

UMultiplayerFPSInGameMenuWidget::UMultiplayerFPSInGameMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UMultiplayerFPSInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_ResumeGame))
	{
		Button_ResumeGame->OnClicked.AddDynamic(this, &UMultiplayerFPSInGameMenuWidget::ResumeButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSInGameMenuWidget::NativeConstruct() -> Button_ResumeGame is not Valid !!!"));
	}

	if (IsValid(Button_QuitGame))
	{
		Button_QuitGame->OnClicked.AddDynamic(this, &UMultiplayerFPSInGameMenuWidget::QuitButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSInGameMenuWidget::NativeConstruct() -> Button_QuitGame is not Valid !!!"));
	}
}

void UMultiplayerFPSInGameMenuWidget::ResumeButtonClicked()
{
	AMultiplayerFPSCharacter* Player = Cast<AMultiplayerFPSCharacter>(GetOwningPlayerPawn());
	if(IsValid(Player))
	{
		Player->ToggleOptionMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSInGameMenuWidget::ResumeButtonClicked() -> Player is not Valid !!!"));
	}
}

void UMultiplayerFPSInGameMenuWidget::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}


