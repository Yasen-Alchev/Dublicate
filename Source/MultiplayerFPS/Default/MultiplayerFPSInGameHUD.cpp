// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSInGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/MultiplayerFPSBuyMenuWidget.h"
#include "Widgets/MultiplayerFPSEndGameScreenWidget.h"
#include "Widgets/MultiplayerFPSGameTimeWidget.h"
#include "Widgets/MultiplayerFPSInGameMenuWidget.h"
#include "Widgets/MultiplayerFPSObjectiveStatsWidget.h"
#include "Widgets/MultiplayerFPSLeaderBoardWidget.h"


AMultiplayerFPSInGameHUD::AMultiplayerFPSInGameHUD() {}

void AMultiplayerFPSInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(LeaderBoardWidgetClass))
	{
		LeaderBoardWidget = CreateWidget<UMultiplayerFPSLeaderBoardWidget>(GetWorld(), LeaderBoardWidgetClass);
		if (IsValid(LeaderBoardWidget))
		{
			LeaderBoardWidget->SetVisibility(ESlateVisibility::Hidden);
			LeaderBoardWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> LeaderBoardWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> LeaderBoardWidgetClass is not Valid!!!"));
	}

	if (IsValid(ObjectiveStatsWidgetClass))
	{
		ObjectiveStatsWidget = CreateWidget<UMultiplayerFPSObjectiveStatsWidget>(GetWorld(), ObjectiveStatsWidgetClass);
		if (IsValid(ObjectiveStatsWidget))
		{
			ObjectiveStatsWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> ObjectiveStatsWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> ObjectiveStatsWidgetClass is not Valid!!!"));
	}

	if (IsValid(GameTimeWidgetClass))
	{
		GameTimeWidget = CreateWidget<UMultiplayerFPSGameTimeWidget>(GetWorld(), GameTimeWidgetClass);
		if (IsValid(GameTimeWidget))
		{
			GameTimeWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> GameTimeWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> GameTimeWidgetClass is not Valid!!!"));
	}

	if (IsValid(EndGameScreenWidgetClass))
	{
		EndGameScreenWidget = CreateWidget<UMultiplayerFPSEndGameScreenWidget>(GetWorld(), EndGameScreenWidgetClass);
		if (IsValid(EndGameScreenWidget))
		{
			EndGameScreenWidget->SetVisibility(ESlateVisibility::Hidden);
			EndGameScreenWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> EndGameScreenWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> EndGameScreenWidgetClass is not Valid!!!"));
	}

	if (IsValid(BuyMenuWidgetClass))
	{
		BuyMenuWidget = CreateWidget<UMultiplayerFPSBuyMenuWidget>(GetWorld(), BuyMenuWidgetClass);
		if (IsValid(BuyMenuWidget))
		{
			BuyMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			BuyMenuWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> BuyMenuWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> BuyMenuWidgetClass is not Valid!!!"));
	}

	if (IsValid(InGameMenuWidgetClass))
	{
		InGameMenuWidget = CreateWidget<UMultiplayerFPSInGameMenuWidget>(GetWorld(), InGameMenuWidgetClass);
		if (IsValid(InGameMenuWidget))
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			InGameMenuWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> InGameMenuWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> InGameMenuWidgetClass is not Valid!!!"));
	}
}


void AMultiplayerFPSInGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMultiplayerFPSInGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AMultiplayerFPSInGameHUD::UpdateObjectiveStats(int32 RedScore, int32 BlueScore)
{
	if (RedScore >= 0 || BlueScore >= 0)
	{
		if (IsValid(ObjectiveStatsWidget))
		{
			ObjectiveStatsWidget->UpdateStats(RedScore, BlueScore);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats(int32 RedScore, int32 BlueScore) -> ObjectiveStatsWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats(int32 RedScore, int32 BlueScore) -> RedScore or BlueScore was less than Zero"));
	}
}

void AMultiplayerFPSInGameHUD::UpdateGameTime(int minutes, int seconds)
{
	if (IsValid(GameTimeWidget))
	{
		GameTimeWidget->UpdateGameTime(minutes, seconds);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateGameTime(int minutes, int seconds) -> GameTimeWidget is Invalid !!!"));
	}
}

void AMultiplayerFPSInGameHUD::ResetObjectiveStats()
{
	if (IsValid(ObjectiveStatsWidget))
	{
		ObjectiveStatsWidget->ResetStats();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::ResetObjectiveStats() -> ObjectiveStatsWidget is not Valid!!!"));
	}
}

void AMultiplayerFPSInGameHUD::SetOptionMenuVisibility(bool Visibility)
{
	if (IsValid(InGameMenuWidget))
	{
		if (Visibility)
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::SetOptionsMenuVisibility(bool Visibility) -> InGameMenuWidget is not Valid !!!"));
	}
}

void AMultiplayerFPSInGameHUD::ToggleLeaderBoardVisibility()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("AMultiplayerFPSInGameHUD::ToggleLeaderBoardVisibility()!!!!!!!!!!!!!!!!!!!!")));

	if (IsValid(LeaderBoardWidget))
	{
		if (LeaderBoardWidget->IsVisible())
		{
			//UE_LOG(LogTemp, Error, TEXT("LeaderBoardWidget IS VISIBLE !!!"));
			LeaderBoardWidget->RemoveFromParent();
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("LeaderBoardWidget IS NOT VISIBLE !!!"));
			UpdateLeaderBoardStats();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::ToggleLeaderBoardVisibility() -> LeaderBoardWidget is not Valid!!!"));
	}
}

void AMultiplayerFPSInGameHUD::UpdateLeaderBoardStats()
{
	if (IsValid(LeaderBoardWidget))
	{
		bool isVisible = LeaderBoardWidget->GetVisibility() == ESlateVisibility::Visible;
		LeaderBoardWidget->RemoveFromParent();
		LeaderBoardWidget = CreateWidget<UMultiplayerFPSLeaderBoardWidget>(GetWorld(), LeaderBoardWidgetClass);
		if (IsValid(LeaderBoardWidget))
		{
			isVisible ? LeaderBoardWidget->SetVisibility(ESlateVisibility::Visible) : LeaderBoardWidget->SetVisibility(ESlateVisibility::Hidden);
			LeaderBoardWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::UpdateLeaderBoardStats() -> LeaderBoardWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateLeaderBoardStats() -> LeaderBoardWidget is not Valid!!!"));
	}
}

void AMultiplayerFPSInGameHUD::GameEnded(ETeams WinnerTeam)
{
	if (IsValid(EndGameScreenWidget))
	{
		EndGameScreenWidget->SetWinnerTeam(WinnerTeam);
		EndGameScreenWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::GameEnded(ETeams WinnerTeam) -> EndGameScreenWidget is not Valid!!!"));
	}

	if (IsValid(InGameMenuWidget))
	{
		InGameMenuWidget->RemoveFromViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::GameEnded(ETeams WinnerTeam) -> InGameMenuWidget is not Valid!!!"));
	}
}



