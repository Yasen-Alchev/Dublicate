// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSLeaderBoardUnitWidget.h"

UMultiplayerFPSLeaderBoardUnitWidget::UMultiplayerFPSLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UMultiplayerFPSLeaderBoardUnitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMultiplayerFPSLeaderBoardUnitWidget::SetPlayerName(FString Name)
{
	if(IsValid(TXTBlock_PlayerName))
	{
		TXTBlock_PlayerName->SetText(FText::FromString(Name));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardUnitWidget::SetPlayerName(FString Name) -> TXTBlock_PlayerName is not Valid !!!"));
	}
}

void UMultiplayerFPSLeaderBoardUnitWidget::SetPlayerKills(FString Kills)
{
	if (IsValid(TXTBlock_PlayerKills))
	{
		TXTBlock_PlayerKills->SetText(FText::FromString(Kills));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardUnitWidget::SetPlayerKills(FString Kills) -> TXTBlock_PlayerKills is not Valid !!!"));
	}
}

void UMultiplayerFPSLeaderBoardUnitWidget::SetPlayerDeaths(FString Deaths)
{
	if (IsValid(TXTBlock_PlayerDeaths))
	{
		TXTBlock_PlayerDeaths->SetText(FText::FromString(Deaths));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardUnitWidget::SetPlayerDeaths(FString Deaths) -> TXTBlock_PlayerDeaths is not Valid !!!"));
	}
}

void UMultiplayerFPSLeaderBoardUnitWidget::SetPlayerScore(FString Score)
{
	if (IsValid(TXTBlock_PlayerScore))
	{
		TXTBlock_PlayerScore->SetText(FText::FromString(Score));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardUnitWidget::SetPlayerScore(FString Score) -> TXTBlock_PlayerScore is not Valid !!!"));
	}
}
