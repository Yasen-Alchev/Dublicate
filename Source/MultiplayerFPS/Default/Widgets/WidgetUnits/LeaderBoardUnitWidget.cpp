// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderBoardUnitWidget.h"

ULeaderBoardUnitWidget::ULeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void ULeaderBoardUnitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULeaderBoardUnitWidget::SetPlayerName(FString Name)
{
	if(!IsValid(TXTBlock_PlayerName))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardUnitWidget::SetPlayerName(FString Name) -> TXTBlock_PlayerName is not Valid !!!"));
		return;
	}
	TXTBlock_PlayerName->SetText(FText::FromString(Name));
}

void ULeaderBoardUnitWidget::SetPlayerKills(FString Kills)
{
	if (!IsValid(TXTBlock_PlayerKills))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardUnitWidget::SetPlayerKills(FString Kills) -> TXTBlock_PlayerKills is not Valid !!!"));
		return;
	}
	TXTBlock_PlayerKills->SetText(FText::FromString(Kills));
}

void ULeaderBoardUnitWidget::SetPlayerDeaths(FString Deaths)
{
	if (!IsValid(TXTBlock_PlayerDeaths))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardUnitWidget::SetPlayerDeaths(FString Deaths) -> TXTBlock_PlayerDeaths is not Valid !!!"));
		return;
	}
	TXTBlock_PlayerDeaths->SetText(FText::FromString(Deaths));
}

void ULeaderBoardUnitWidget::SetPlayerScore(FString Score)
{
	if (!IsValid(TXTBlock_PlayerScore))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardUnitWidget::SetPlayerScore(FString Score) -> TXTBlock_PlayerScore is not Valid !!!"));
		return;
	}
	TXTBlock_PlayerScore->SetText(FText::FromString(Score));
}

FString ULeaderBoardUnitWidget::GetPlayerName()
{
	return TXTBlock_PlayerName->GetText().ToString();
}

FString ULeaderBoardUnitWidget::GetPlayerKills()
{
	return TXTBlock_PlayerKills->GetText().ToString();
}

FString ULeaderBoardUnitWidget::GetPlayerDeaths()
{
	return TXTBlock_PlayerDeaths->GetText().ToString();
}

FString ULeaderBoardUnitWidget::GetPlayerScore()
{
	return TXTBlock_PlayerScore->GetText().ToString();
}
