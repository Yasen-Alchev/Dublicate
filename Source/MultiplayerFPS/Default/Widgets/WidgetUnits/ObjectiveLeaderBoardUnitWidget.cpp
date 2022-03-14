// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveLeaderBoardUnitWidget.h"

UObjectiveLeaderBoardUnitWidget::UObjectiveLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UObjectiveLeaderBoardUnitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UObjectiveLeaderBoardUnitWidget::SetPlayerObjectiveScore(FString Score)
{
	if (IsValid(TXTBlock_PlayerObjectiveScore))
	{
		TXTBlock_PlayerObjectiveScore->SetText(FText::FromString(Score));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectiveLeaderBoardUnitWidget::SetPlayerObjectiveScore(FString Score) -> TXTBlock_PlayerObjectiveScore is not Valid !!!"));
	}
}
