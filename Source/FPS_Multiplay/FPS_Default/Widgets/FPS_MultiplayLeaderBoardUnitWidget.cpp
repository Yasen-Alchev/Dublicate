// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayLeaderBoardUnitWidget.h"

UFPS_MultiplayLeaderBoardUnitWidget::UFPS_MultiplayLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UFPS_MultiplayLeaderBoardUnitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFPS_MultiplayLeaderBoardUnitWidget::SetPlayerName(FString Name)
{
	if(IsValid(TXTBlock_PlayerName))
	{
		TXTBlock_PlayerName->SetText(FText::FromString(Name));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardUnitWidget::SetPlayerName(FString Name) -> TXTBlock_PlayerName is not Valid !!!"));
	}
}

void UFPS_MultiplayLeaderBoardUnitWidget::SetPlayerKills(FString Kills)
{
	if (IsValid(TXTBlock_PlayerKills))
	{
		TXTBlock_PlayerKills->SetText(FText::FromString(Kills));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardUnitWidget::SetPlayerKills(FString Kills) -> TXTBlock_PlayerKills is not Valid !!!"));
	}
}

void UFPS_MultiplayLeaderBoardUnitWidget::SetPlayerDeaths(FString Deaths)
{
	if (IsValid(TXTBlock_PlayerDeaths))
	{
		TXTBlock_PlayerDeaths->SetText(FText::FromString(Deaths));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardUnitWidget::SetPlayerDeaths(FString Deaths) -> TXTBlock_PlayerDeaths is not Valid !!!"));
	}
}

void UFPS_MultiplayLeaderBoardUnitWidget::SetPlayerScore(FString Score)
{
	if (IsValid(TXTBlock_PlayerScore))
	{
		TXTBlock_PlayerScore->SetText(FText::FromString(Score));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardUnitWidget::SetPlayerScore(FString Score) -> TXTBlock_PlayerScore is not Valid !!!"));
	}
}
