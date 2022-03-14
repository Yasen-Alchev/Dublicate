// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveStatsWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UObjectiveStatsWidget::UObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UObjectiveStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ResetStats();
}

void UObjectiveStatsWidget::UpdateStats(FString RedScore, FString BlueScore)
{
	if (IsValid(TXTBlock_RedScoreStat) && IsValid(TXTBlock_BlueScoreStat))
	{
		TXTBlock_RedScoreStat->SetText(FText::FromString(RedScore));
		TXTBlock_BlueScoreStat->SetText(FText::FromString(BlueScore));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectiveStatsWidget::UpdateStats(int32 RedScore, int32 BlueScore) -> TXTBlock_RedScoreStat or  TXTBlock_BlueScoreStat is Invalid or RedScore or BlueScore is less than Zero !!!"));
	}
}

void UObjectiveStatsWidget::ResetStats()
{
	if (IsValid(TXTBlock_RedScoreStat) && IsValid(TXTBlock_BlueScoreStat))
	{
		TXTBlock_RedScoreStat->SetText(FText::FromString("Red: 0"));
		TXTBlock_BlueScoreStat->SetText(FText::FromString("Blue: 0"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectiveStatsWidget::ResetStats() -> TXTBlock_RedScoreStat or  TXTBlock_BlueScoreStat is not Valid!!!"));
	}
}

void UObjectiveStatsWidget::SetGameLeader(FString LeaderName)
{
	if (IsValid(TXTBlock_GameLeader))
	{
		TXTBlock_GameLeader->SetText(FText::FromString(LeaderName));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectiveStatsWidget::SetGameLeader(FString LeaderName) -> TXTBlock_GameLeader is not Valid!!!"));
	}
}

void UObjectiveStatsWidget::ResetGameLeader()
{
	if (IsValid(TXTBlock_GameLeader))
	{
		TXTBlock_GameLeader->SetText(FText());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectiveStatsWidget::ResetGameLeader() -> TXTBlock_GameLeader is not Valid!!!"));
	}
}
