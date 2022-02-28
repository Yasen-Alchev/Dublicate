// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayInGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/FPS_MultiplayBuyMenuWidget.h"
#include "Widgets/FPS_MultiplayEndGameScreenWidget.h"
#include "Widgets/FPS_MultiplayGameTimeWidget.h"
#include "Widgets/FPS_MultiplayInGameMenuWidget.h"
#include "Widgets/FPS_MultiplayObjectiveStatsWidget.h"
#include "Widgets/FPS_MultiplayLeaderBoardWidget.h"


AFPS_MultiplayInGameHUD::AFPS_MultiplayInGameHUD() {}

void AFPS_MultiplayInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(LeaderBoardWidgetClass))
	{
		LeaderBoardWidget = CreateWidget<UFPS_MultiplayLeaderBoardWidget>(GetWorld(), LeaderBoardWidgetClass);
		if (IsValid(LeaderBoardWidget))
		{
			LeaderBoardWidget->SetVisibility(ESlateVisibility::Hidden);
			LeaderBoardWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> LeaderBoardWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> LeaderBoardWidgetClass is not Valid!!!"));
	}

	if (IsValid(ObjectiveStatsWidgetClass))
	{
		ObjectiveStatsWidget = CreateWidget<UFPS_MultiplayObjectiveStatsWidget>(GetWorld(), ObjectiveStatsWidgetClass);
		if (IsValid(ObjectiveStatsWidget))
		{
			ObjectiveStatsWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> ObjectiveStatsWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> ObjectiveStatsWidgetClass is not Valid!!!"));
	}

	if (IsValid(GameTimeWidgetClass))
	{
		GameTimeWidget = CreateWidget<UFPS_MultiplayGameTimeWidget>(GetWorld(), GameTimeWidgetClass);
		if (IsValid(GameTimeWidget))
		{
			GameTimeWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> GameTimeWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> GameTimeWidgetClass is not Valid!!!"));
	}

	if (IsValid(EndGameScreenWidgetClass))
	{
		EndGameScreenWidget = CreateWidget<UFPS_MultiplayEndGameScreenWidget>(GetWorld(), EndGameScreenWidgetClass);
		if (IsValid(EndGameScreenWidget))
		{
			EndGameScreenWidget->SetVisibility(ESlateVisibility::Hidden);
			EndGameScreenWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> EndGameScreenWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> EndGameScreenWidgetClass is not Valid!!!"));
	}

	if (IsValid(BuyMenuWidgetClass))
	{
		BuyMenuWidget = CreateWidget<UFPS_MultiplayBuyMenuWidget>(GetWorld(), BuyMenuWidgetClass);
		if (IsValid(BuyMenuWidget))
		{
			BuyMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			BuyMenuWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> BuyMenuWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> BuyMenuWidgetClass is not Valid!!!"));
	}

	if (IsValid(InGameMenuWidgetClass))
	{
		InGameMenuWidget = CreateWidget<UFPS_MultiplayInGameMenuWidget>(GetWorld(), InGameMenuWidgetClass);
		if (IsValid(InGameMenuWidget))
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			InGameMenuWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> InGameMenuWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::BeginPlay() -> InGameMenuWidgetClass is not Valid!!!"));
	}
}


void AFPS_MultiplayInGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AFPS_MultiplayInGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AFPS_MultiplayInGameHUD::UpdateObjectiveStats(int32 RedScore, int32 BlueScore)
{
	if (RedScore >= 0 || BlueScore >= 0)
	{
		if (IsValid(ObjectiveStatsWidget))
		{
			ObjectiveStatsWidget->UpdateStats(RedScore, BlueScore);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::UpdateObjectiveStats(int32 RedScore, int32 BlueScore) -> ObjectiveStatsWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AFPS_MultiplayInGameHUD::UpdateObjectiveStats(int32 RedScore, int32 BlueScore) -> RedScore or BlueScore was less than Zero"));
	}
}

void AFPS_MultiplayInGameHUD::UpdateGameTime(int minutes, int seconds)
{
	if (IsValid(GameTimeWidget))
	{
		GameTimeWidget->UpdateGameTime(minutes, seconds);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AFPS_MultiplayInGameHUD::UpdateGameTime(int minutes, int seconds) -> GameTimeWidget is Invalid !!!"));
	}
}

void AFPS_MultiplayInGameHUD::ResetObjectiveStats()
{
	if (IsValid(ObjectiveStatsWidget))
	{
		ObjectiveStatsWidget->ResetStats();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AFPS_MultiplayInGameHUD::ResetObjectiveStats() -> ObjectiveStatsWidget is not Valid!!!"));
	}
}

void AFPS_MultiplayInGameHUD::SetOptionMenuVisibility(bool Visibility)
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
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::SetOptionsMenuVisibility(bool Visibility) -> InGameMenuWidget is not Valid !!!"));
	}
}

void AFPS_MultiplayInGameHUD::ToggleLeaderBoardVisibility()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("AFPS_MultiplayInGameHUD::ToggleLeaderBoardVisibility()!!!!!!!!!!!!!!!!!!!!")));

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
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::ToggleLeaderBoardVisibility() -> LeaderBoardWidget is not Valid!!!"));
	}
}

void AFPS_MultiplayInGameHUD::UpdateLeaderBoardStats()
{
	if (IsValid(LeaderBoardWidget))
	{
		LeaderBoardWidget->RemoveFromParent();
		LeaderBoardWidget = CreateWidget<UFPS_MultiplayLeaderBoardWidget>(GetWorld(), LeaderBoardWidgetClass);
		if (IsValid(LeaderBoardWidget))
		{
			LeaderBoardWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::UpdateLeaderBoardStats() -> LeaderBoardWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AFPS_MultiplayInGameHUD::UpdateLeaderBoardStats() -> LeaderBoardWidget is not Valid!!!"));
	}
}

void AFPS_MultiplayInGameHUD::GameEnded(EFPS_Teams WinnerTeam)
{
	if (IsValid(EndGameScreenWidget))
	{
		EndGameScreenWidget->SetWinnerTeam(WinnerTeam);
		EndGameScreenWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::GameEnded(EFPS_Teams WinnerTeam) -> EndGameScreenWidget is not Valid!!!"));
	}

	if (IsValid(InGameMenuWidget))
	{
		InGameMenuWidget->RemoveFromViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AFPS_MultiplayInGameHUD::GameEnded(EFPS_Teams WinnerTeam) -> InGameMenuWidget is not Valid!!!"));
	}
}



