// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayObjectiveLeaderBoardUnitWidget.h"

UFPS_MultiplayObjectiveLeaderBoardUnitWidget::UFPS_MultiplayObjectiveLeaderBoardUnitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UFPS_MultiplayObjectiveLeaderBoardUnitWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFPS_MultiplayObjectiveLeaderBoardUnitWidget::SetPlayerObjectiveScore(FString Score)
{
	if (IsValid(TXTBlock_PlayerObjectiveScore))
	{
		TXTBlock_PlayerObjectiveScore->SetText(FText::FromString(Score));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayObjectiveLeaderBoardUnitWidget::SetPlayerObjectiveScore(FString Score) -> TXTBlock_PlayerObjectiveScore is not Valid !!!"));
	}
}
