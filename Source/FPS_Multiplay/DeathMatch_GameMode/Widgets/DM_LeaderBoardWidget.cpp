// Fill out your copyright notice in the Description page of Project Settings.


#include "DM_LeaderBoardWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UDM_LeaderBoardWidget::UDM_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UDM_LeaderBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ResetStats();
}

void UDM_LeaderBoardWidget::UpdateStats(int Kills, int Deaths)
{
	if (IsValid(TXTBlock_Kills) && Kills >= 0)
	{
		TXTBlock_Kills->SetText(FText::FromString("Kills: " + FString::FromInt(Kills)));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_LeaderBoardWidget::UpdateStats(int Kills, int Deaths) -> TXTBlock_Kills is not Valid or Kills is less than Zero!!!"));
	}

	if (IsValid(TXTBlock_Deaths) && Deaths >= 0)
	{
		TXTBlock_Deaths->SetText(FText::FromString("Deaths: " + FString::FromInt(Deaths)));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_LeaderBoardWidget::UpdateStats(int Kills, int Deaths) -> TXTBlock_Deaths is not Valid or Deaths is less than Zero!!!"));
	}
}

void UDM_LeaderBoardWidget::ResetStats()
{
	if (IsValid(TXTBlock_Kills))
	{
		TXTBlock_Kills->SetText(FText::FromString("Kills: 0"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_LeaderBoardWidget::ResetStats() -> TXTBlock_Kills is not Valid !!!"));
	}

	if (IsValid(TXTBlock_Deaths))
	{
		TXTBlock_Deaths->SetText(FText::FromString("Deaths: 0"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_LeaderBoardWidget::ResetStats() -> TXTBlock_Deaths  is not Valid !!!"));
	}
}