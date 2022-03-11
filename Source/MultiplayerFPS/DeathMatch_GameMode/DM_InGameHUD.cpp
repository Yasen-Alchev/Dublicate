#include "DM_InGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/GameTimeWidget.h"
#include "MultiplayerFPS/Default/Widgets/InGameMenuWidget.h"
#include "Widgets/DM_BuyMenuWidget.h"
#include "Widgets/DM_EndGameScreenWidget.h"
#include "Widgets/DM_ObjectiveStatsWidget.h"
#include "Widgets/DM_LeaderBoardWidget.h"

ADM_InGameHUD::ADM_InGameHUD() {}

void ADM_InGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ADM_InGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(LeaderBoardWidgetClass))
	{
		LeaderBoardWidget = CreateWidget<UDM_LeaderBoardWidget>(GetWorld(), LeaderBoardWidgetClass);
		if (IsValid(LeaderBoardWidget))
		{
			LeaderBoardWidget->SetVisibility(ESlateVisibility::Hidden);
			LeaderBoardWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> LeaderBoardWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> LeaderBoardWidgetClass is not Valid!!!"));
	}

	if (IsValid(ObjectiveStatsWidgetClass))
	{
		ObjectiveStatsWidget = CreateWidget<UDM_ObjectiveStatsWidget>(GetWorld(), ObjectiveStatsWidgetClass);
		if (IsValid(ObjectiveStatsWidget))
		{
			ObjectiveStatsWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> ObjectiveStatsWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> ObjectiveStatsWidgetClass is not Valid!!!"));
	}

	if (IsValid(GameTimeWidgetClass))
	{
		GameTimeWidget = CreateWidget <UGameTimeWidget> (GetWorld(), GameTimeWidgetClass);
		if (IsValid(GameTimeWidget))
		{
			GameTimeWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> GameTimeWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> GameTimeWidgetClass is not Valid!!!"));
	}

	if (IsValid(EndGameScreenWidgetClass))
	{
		EndGameScreenWidget = CreateWidget<UDM_EndGameScreenWidget>(GetWorld(), EndGameScreenWidgetClass);
		if (IsValid(EndGameScreenWidget))
		{
			EndGameScreenWidget->SetVisibility(ESlateVisibility::Hidden);
			EndGameScreenWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> EndGameScreenWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> EndGameScreenWidgetClass is not Valid!!!"));
	}

	if (IsValid(BuyMenuWidgetClass))
	{
		BuyMenuWidget = CreateWidget<UDM_BuyMenuWidget>(GetWorld(), BuyMenuWidgetClass);
		if (IsValid(BuyMenuWidget))
		{
			BuyMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			BuyMenuWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> BuyMenuWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> BuyMenuWidgetClass is not Valid!!!"));
	}

	if (IsValid(InGameMenuWidgetClass))
	{
		InGameMenuWidget = CreateWidget<UInGameMenuWidget>(GetWorld(), InGameMenuWidgetClass);
		if (IsValid(InGameMenuWidget))
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			InGameMenuWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> InGameMenuWidget is not Valid!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::BeginPlay() -> InGameMenuWidgetClass is not Valid!!!"));
	}
}

void ADM_InGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADM_InGameHUD::UpdateStats(int Kills, int Deaths)
{
	if (IsValid(LeaderBoardWidget))
	{
		LeaderBoardWidget->UpdateStats(Kills, Deaths);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::UpdateStats(int Kills, int Deaths) -> LeaderBoardWidget is not Valid!!!"));
	}
}

void ADM_InGameHUD::UpdateObjectiveStats(ADM_PlayerState* Player1, ADM_PlayerState* Player2, ADM_PlayerState* Player3)
{
	if (IsValid(ObjectiveStatsWidget))
	{
		ObjectiveStatsWidget->UpdateStats();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::UpdateObjectiveStats() -> ObjectiveStatsWidget is not Valid!!!"));
	}
}

void ADM_InGameHUD::UpdateGameTime(int minutes, int seconds)
{
	if (IsValid(GameTimeWidget))
	{
		GameTimeWidget->UpdateGameTime(minutes, seconds);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADM_InGameHUD::UpdateGameTime(int minutes, int seconds) -> GameTimeWidget is not Valid !!!"));
	}
}

void ADM_InGameHUD::ResetStats()
{
	if (IsValid(LeaderBoardWidget))
	{
		LeaderBoardWidget->ResetStats();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADM_InGameHUD::ResetStats() -> LeaderBoardWidget is not Valid!!!"));
	}
}

void ADM_InGameHUD::ResetObjectiveStats()
{
	if (IsValid(ObjectiveStatsWidget))
	{
		ObjectiveStatsWidget->ResetStats();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADM_InGameHUD::ResetObjectiveStats() -> ObjectiveStatsWidget is not Valid!!!"));
	}
}

void ADM_InGameHUD::SetOptionsMenuVisibility(bool Visibility)
{
	if (IsValid(InGameMenuWidget))
	{
		if(InGameMenuWidget->IsVisible())
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			InGameMenuWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ADM_InGameHUD::SetOptionsMenuVisibility(bool Visibility) -> InGameMenuWidget is not Valid !!!"));
	}
}

void ADM_InGameHUD::ToggleStatsVisibility()
{
	if (IsValid(LeaderBoardWidget))
	{
		if(LeaderBoardWidget->IsVisible())
		{
			LeaderBoardWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			LeaderBoardWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ADM_InGameHUD::ToggleStatsVisibility() -> LeaderBoardWidget is not Valid!!!"));
	}
}

void ADM_InGameHUD::GameEnded(const FString& WinnerName)
{
	if (IsValid(EndGameScreenWidget))
	{
		EndGameScreenWidget->SetWinner(WinnerName);
		EndGameScreenWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::GameEnded() -> EndGameScreenWidget is not Valid!!!"));
	}

	if (IsValid(InGameMenuWidget))
	{
		InGameMenuWidget->RemoveFromViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::GameEnded() -> EndGameScreenWidget is not Valid!!!"));
	}
}

