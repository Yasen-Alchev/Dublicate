// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayInGameMenuWidget.h"
#include "FPS_Multiplay/DeathMatch_GameMode/DM_Character.h"
#include "Runtime/UMG/Public/UMG.h"

UFPS_MultiplayInGameMenuWidget::UFPS_MultiplayInGameMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UFPS_MultiplayInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_ResumeGame))
	{
		Button_ResumeGame->OnClicked.AddDynamic(this, &UFPS_MultiplayInGameMenuWidget::ResumeButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayInGameMenuWidget::NativeConstruct() -> Button_ResumeGame is not Valid !!!"));
	}

	if (IsValid(Button_QuitGame))
	{
		Button_QuitGame->OnClicked.AddDynamic(this, &UFPS_MultiplayInGameMenuWidget::QuitButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayInGameMenuWidget::NativeConstruct() -> Button_QuitGame is not Valid !!!"));
	}
}

void UFPS_MultiplayInGameMenuWidget::ResumeButtonClicked()
{
	AFPS_MultiplayCharacter* Player = Cast<AFPS_MultiplayCharacter>(GetOwningPlayerPawn());
	if(IsValid(Player))
	{
		Player->ToggleOptionMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayInGameMenuWidget::ResumeButtonClicked() -> Player is not Valid !!!"));
	}
}

void UFPS_MultiplayInGameMenuWidget::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}


