#include "MultiplayerFPSInGameHUD.h"

#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "TeamBasedClasses/TeamBasedPlayerController.h"
#include "Widgets/BuyMenuWidget.h"
#include "Widgets/EndGameScreenWidget.h"
#include "Widgets/GameTimeWidget.h"
#include "Widgets/InGameMenuWidget.h"
#include "Widgets/LeaderBoardWidget.h"
#include "Widgets/ObjectiveStatsWidget.h"


AMultiplayerFPSInGameHUD::AMultiplayerFPSInGameHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AMultiplayerFPSInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		if (IsValid(LeaderBoardWidgetClass))
		{
			LeaderBoardWidget = CreateWidget<ULeaderBoardWidget>(World, LeaderBoardWidgetClass);
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
			ObjectiveStatsWidget = CreateWidget<UObjectiveStatsWidget>(World, ObjectiveStatsWidgetClass);
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
			GameTimeWidget = CreateWidget<UGameTimeWidget>(World, GameTimeWidgetClass);
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
			EndGameScreenWidget = CreateWidget<UEndGameScreenWidget>(World, EndGameScreenWidgetClass);
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
			BuyMenuWidget = CreateWidget<UBuyMenuWidget>(World, BuyMenuWidgetClass);
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
			InGameMenuWidget = CreateWidget<UInGameMenuWidget>(World, InGameMenuWidgetClass);
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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> World is not Valid!!!"));
	}
}

void AMultiplayerFPSInGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMultiplayerFPSInGameHUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	const FVector2D CrosshairDrawPosition((Center.X),
		(Center.Y - 5.35f/*+ 20.0f*/));

	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

void AMultiplayerFPSInGameHUD::UpdateObjectiveStats(const TArray<FString>& ObjectiveStats)
{
	if (IsValid(ObjectiveStatsWidget))
	{
		if (TSubclassOf<AMultiplayerFPSPlayerController>(GetOwningPlayerController()->GetClass()))
		{
			if (TSubclassOf<ATeamBasedPlayerController>(GetOwningPlayerController()->GetClass()))
			{
				if (ObjectiveStats.Num() == 2)
				{
					FString RedScore, BlueScore;
					if (ObjectiveStats[0].Contains("Red", ESearchCase::IgnoreCase, ESearchDir::FromStart))
					{
						RedScore = ObjectiveStats[0];
						BlueScore = ObjectiveStats[1];
					}
					else if (ObjectiveStats[1].Contains("Red", ESearchCase::IgnoreCase, ESearchDir::FromStart))
					{
						RedScore = ObjectiveStats[1];
						BlueScore = ObjectiveStats[0];
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() -> ObjectiveStats does not contain substring \"Red\" !!!"));
						return;
					}
					ObjectiveStatsWidget->UpdateStats(RedScore, BlueScore);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() -> ObjectiveStats.Num is not exactly 2 !!!"));
				}
			}
			else if (ObjectiveStats.Num() == 1 && !ObjectiveStats[0].IsEmpty())
			{
				ObjectiveStatsWidget->SetGameLeader(ObjectiveStats[0]);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() -> ObjectiveStats is Empty (No GameLeader provided) or  ObjectiveStats size is not exactly 1!!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() ->  GetOwningPlayerController's Class is Not TSubclassOf of AMultiplayerFPSPlayerController !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() ->  ObjectiveStatsWidget is not Valid !!!"));
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

void AMultiplayerFPSInGameHUD::ResetGameLeader()
{
	if (IsValid(ObjectiveStatsWidget))
	{
		ObjectiveStatsWidget->ResetGameLeader();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::ResetGameLeader() -> ObjectiveStatsWidget is not Valid!!!"));
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
	if (IsValid(LeaderBoardWidget))
	{
		if (LeaderBoardWidget->IsVisible())
		{
			LeaderBoardWidget->RemoveFromParent();
		}
		else
		{
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
		LeaderBoardWidget->SetVisibility(ESlateVisibility::Hidden);
		LeaderBoardWidget->RemoveFromParent();
		LeaderBoardWidget = CreateWidget<ULeaderBoardWidget>(GetWorld(), LeaderBoardWidgetClass);
		if (IsValid(LeaderBoardWidget))
		{
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

void AMultiplayerFPSInGameHUD::GameEnded(const FString& Winner)
{
	if (IsValid(EndGameScreenWidget))
	{
		EndGameScreenWidget->SetWinnerTeam(Winner);
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



