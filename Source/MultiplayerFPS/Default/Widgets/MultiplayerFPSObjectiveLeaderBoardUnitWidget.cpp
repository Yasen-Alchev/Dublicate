// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSObjectiveLeaderBoardUnitWidget.h"

UMultiplayerFPSObjectiveLeaderBoardUnitWidget::UMultiplayerFPSObjectiveLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UMultiplayerFPSObjectiveLeaderBoardUnitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMultiplayerFPSObjectiveLeaderBoardUnitWidget::SetPlayerObjectiveScore(FString Score)
{
	if (IsValid(TXTBlock_PlayerObjectiveScore))
	{
		TXTBlock_PlayerObjectiveScore->SetText(FText::FromString(Score));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSObjectiveLeaderBoardUnitWidget::SetPlayerObjectiveScore(FString Score) -> TXTBlock_PlayerObjectiveScore is not Valid !!!"));
	}
}
