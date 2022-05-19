// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenuWidget.h"

#include "MultiplayerFPS/Conquest_GameMode/CQ_Character.h"
#include "MultiplayerFPS/CTF_GameMode/CTF_Character.h"
#include "MultiplayerFPS/DeathMatch_GameMode/DM_Character.h"
#include "Runtime/UMG/Public/UMG.h"

UInGameMenuWidget::UInGameMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button_ResumeGame))
	{
		Button_ResumeGame->OnClicked.AddDynamic(this, &UInGameMenuWidget::ResumeButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInGameMenuWidget::NativeConstruct() -> Button_ResumeGame is not Valid !!!"));
	}

	if (IsValid(Button_QuitGame))
	{
		Button_QuitGame->OnClicked.AddDynamic(this, &UInGameMenuWidget::QuitButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInGameMenuWidget::NativeConstruct() -> Button_QuitGame is not Valid !!!"));
	}
}

void UInGameMenuWidget::ResumeButtonClicked()
{
	UClass* UnknownClass = GetOwningPlayerPawn()->GetClass();

	if (TSubclassOf<ADM_Character>(UnknownClass))
	{
		ADM_Character* ADM_Player = Cast<ADM_Character>(GetOwningPlayerPawn());
		if (!IsValid(ADM_Player))
		{
			UE_LOG(LogTemp, Error, TEXT("UInGameMenuWidget::ResumeButtonClicked() -> ADM_Player is not Valid !!!"));
			return;
		}
		ADM_Player->ToggleOptionsMenu();
	}
	else if (TSubclassOf<ACQ_Character>(UnknownClass))
	{
		ACQ_Character* ACQ_Player = Cast<ACQ_Character>(GetOwningPlayerPawn());
		if (!IsValid(ACQ_Player))
		{
			UE_LOG(LogTemp, Error, TEXT("UFPS_InGameMenuWidget::ResumeButtonClicked() -> ACQ_Player is not Valid !!!"));
			return;
		}
		ACQ_Player->ToggleOptionsMenu();
	}
	else if (TSubclassOf<ACTF_Character>(UnknownClass))
	{
		ACTF_Character* ACTF_Player = Cast<ACTF_Character>(GetOwningPlayerPawn());
		if (!IsValid(ACTF_Player))
		{
			UE_LOG(LogTemp, Error, TEXT("UInGameMenuWidget::ResumeButtonClicked() -> ACTF_Player is not Valid !!!"));
			return;
		}
		ACTF_Player->ToggleOptionsMenu();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInGameMenuWidget::ResumeButtonClicked() -> UnknownClass no derived class found of that type !!!"));
	}

}

void UInGameMenuWidget::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}


