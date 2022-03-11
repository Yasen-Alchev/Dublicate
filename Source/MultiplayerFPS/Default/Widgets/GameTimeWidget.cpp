// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimeWidget.h"
#include "Runtime/UMG/Public/UMG.h"


UGameTimeWidget::UGameTimeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UGameTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameTimeWidget::UpdateGameTime(int minutes, int seconds)
{
	if (IsValid(TXTBlock_GameTime))
	{
		TXTBlock_GameTime->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), minutes, seconds)));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UGameTimeWidget::UpdateGameTime(int minutes, int seconds) -> TXTBlock_GameTime is not Valid !!! "));
	}
}

