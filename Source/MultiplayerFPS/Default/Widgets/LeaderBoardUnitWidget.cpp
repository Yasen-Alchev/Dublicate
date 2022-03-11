// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderBoardUnitWidget.h"

ULeaderBoardUnitWidget::ULeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void ULeaderBoardUnitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULeaderBoardUnitWidget::SetPlayerName(FString Name)
{
	if(IsValid(TXTBlock_PlayerName))
	{
		TXTBlock_PlayerName->SetText(FText::FromString(Name));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardUnitWidget::SetPlayerName(FString Name) -> TXTBlock_PlayerName is not Valid !!!"));
	}
}

void ULeaderBoardUnitWidget::SetPlayerKills(FString Kills)
{
	if (IsValid(TXTBlock_PlayerKills))
	{
		TXTBlock_PlayerKills->SetText(FText::FromString(Kills));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardUnitWidget::SetPlayerKills(FString Kills) -> TXTBlock_PlayerKills is not Valid !!!"));
	}
}

void ULeaderBoardUnitWidget::SetPlayerDeaths(FString Deaths)
{
	if (IsValid(TXTBlock_PlayerDeaths))
	{
		TXTBlock_PlayerDeaths->SetText(FText::FromString(Deaths));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardUnitWidget::SetPlayerDeaths(FString Deaths) -> TXTBlock_PlayerDeaths is not Valid !!!"));
	}
}

void ULeaderBoardUnitWidget::SetPlayerScore(FString Score)
{
	if (IsValid(TXTBlock_PlayerScore))
	{
		TXTBlock_PlayerScore->SetText(FText::FromString(Score));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardUnitWidget::SetPlayerScore(FString Score) -> TXTBlock_PlayerScore is not Valid !!!"));
	}
}
