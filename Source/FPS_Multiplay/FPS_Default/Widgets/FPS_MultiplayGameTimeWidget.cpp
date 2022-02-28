// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayGameTimeWidget.h"
#include "Runtime/UMG/Public/UMG.h"


UFPS_MultiplayGameTimeWidget::UFPS_MultiplayGameTimeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UFPS_MultiplayGameTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFPS_MultiplayGameTimeWidget::UpdateGameTime(int minutes, int seconds)
{
	if (IsValid(TXTBlock_GameTime))
	{
		TXTBlock_GameTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), minutes, seconds)));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayGameTimeWidget::UpdateGameTime(int minutes, int seconds) -> TXTBlock_GameTime is not Valid !!! "));
	}
}

