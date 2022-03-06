// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSGameTimeWidget.h"
#include "Runtime/UMG/Public/UMG.h"


UMultiplayerFPSGameTimeWidget::UMultiplayerFPSGameTimeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UMultiplayerFPSGameTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMultiplayerFPSGameTimeWidget::UpdateGameTime(int minutes, int seconds)
{
	if (IsValid(TXTBlock_GameTime))
	{
		TXTBlock_GameTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), minutes, seconds)));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSGameTimeWidget::UpdateGameTime(int minutes, int seconds) -> TXTBlock_GameTime is not Valid !!! "));
	}
}

