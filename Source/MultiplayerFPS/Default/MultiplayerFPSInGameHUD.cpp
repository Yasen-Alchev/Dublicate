#include "MultiplayerFPSInGameHUD.h"

#include "CanvasItem.h"
#include "MultiplayerFPSPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "TeamBasedClasses/TeamBasedPlayerController.h"
#include "Widgets/BuyMenuWidget.h"
#include "Widgets/EndGameScreenWidget.h"
#include "Widgets/GameTimeWidget.h"
#include "Widgets/InGameMenuWidget.h"
#include "Widgets/LeaderBoardWidget.h"
#include "Widgets/ObjectiveStatsWidget.h"
#include "Widgets/WeaponSlotsWidget.h"


AMultiplayerFPSInGameHUD::AMultiplayerFPSInGameHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/ThirdPersonCPP/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AMultiplayerFPSInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> World is not Valid!!!"));
		return;
	}

	if (!IsValid(LeaderBoardWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> LeaderBoardWidgetClass is not Valid!!!"));
		return;
	}
	LeaderBoardWidget = CreateWidget<ULeaderBoardWidget>(World, LeaderBoardWidgetClass);
	if (!IsValid(LeaderBoardWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> LeaderBoardWidget is not Valid!!!"));
		return;
	}
	LeaderBoardWidget->SetVisibility(ESlateVisibility::Hidden);
	LeaderBoardWidget->AddToViewport();

	if (!IsValid(ObjectiveStatsWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> ObjectiveStatsWidgetClass is not Valid!!!"));
		return;
	}
	ObjectiveStatsWidget = CreateWidget<UObjectiveStatsWidget>(World, ObjectiveStatsWidgetClass);
	if (!IsValid(ObjectiveStatsWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> ObjectiveStatsWidget is not Valid!!!"));
		return;
	}
	ObjectiveStatsWidget->AddToViewport();

	if (!IsValid(GameTimeWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> GameTimeWidgetClass is not Valid!!!"));
		return;
	}
	GameTimeWidget = CreateWidget<UGameTimeWidget>(World, GameTimeWidgetClass);
	if (!IsValid(GameTimeWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> GameTimeWidget is not Valid!!!"));
		return;;
	}
	GameTimeWidget->AddToViewport();

	if (!IsValid(EndGameScreenWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> EndGameScreenWidgetClass is not Valid!!!"));
		return;
	}
	EndGameScreenWidget = CreateWidget<UEndGameScreenWidget>(World, EndGameScreenWidgetClass);
	if (!IsValid(EndGameScreenWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> EndGameScreenWidget is not Valid!!!"));
		return;
	}
	EndGameScreenWidget->SetVisibility(ESlateVisibility::Hidden);
	EndGameScreenWidget->AddToViewport();

	if (!IsValid(BuyMenuWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> BuyMenuWidgetClass is not Valid!!!"));
		return;
	}
	BuyMenuWidget = CreateWidget<UBuyMenuWidget>(World, BuyMenuWidgetClass);
	if (!IsValid(BuyMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> BuyMenuWidget is not Valid!!!"));
		return;
	}
	BuyMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	BuyMenuWidget->AddToViewport();

	if (!IsValid(InGameMenuWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> InGameMenuWidgetClass is not Valid!!!"));
	}
	InGameMenuWidget = CreateWidget<UInGameMenuWidget>(World, InGameMenuWidgetClass);
	if (!IsValid(InGameMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> InGameMenuWidget is not Valid!!!"));
		return;
	}
	InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	InGameMenuWidget->AddToViewport();

	if (!IsValid(WeaponSlotsWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> WeaponSlotsWidgetClass is not Valid!!!"));
	}
	WeaponSlotsWidget = CreateWidget<UWeaponSlotsWidget>(World, WeaponSlotsWidgetClass);
	if (!IsValid(WeaponSlotsWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::BeginPlay() -> WeaponSlotsWidget is not Valid!!!"));
		return;
	}
	WeaponSlotsWidget->AddToViewport();
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
	if (!IsValid(ObjectiveStatsWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() ->  ObjectiveStatsWidget is not Valid !!!"));
		return;
	}

	if (!TSubclassOf<AMultiplayerFPSPlayerController>(GetOwningPlayerController()->GetClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() ->  GetOwningPlayerController's Class is Not TSubclassOf of AMultiplayerFPSPlayerController !!!"));
		return;
	}

	if (TSubclassOf<ATeamBasedPlayerController>(GetOwningPlayerController()->GetClass()))
	{
		if (ObjectiveStats.Num() != 2)
		{
			UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() -> ObjectiveStats.Num = %d   Is not exactly 2 !!!"), ObjectiveStats.Num());
			return;
		}

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
	else if (ObjectiveStats.Num() == 1 && !ObjectiveStats[0].IsEmpty())
	{
		ObjectiveStatsWidget->SetGameLeader(ObjectiveStats[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateObjectiveStats() -> ObjectiveStats is Empty (No GameLeader provided) or  ObjectiveStats size is not exactly 1!!!"));
	}
}

void AMultiplayerFPSInGameHUD::UpdateGameTime(int minutes, int seconds)
{
	if (!IsValid(GameTimeWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateGameTime(int minutes, int seconds) -> GameTimeWidget is Invalid !!!"));
		return;
	}
	GameTimeWidget->UpdateGameTime(minutes, seconds);
}

void AMultiplayerFPSInGameHUD::ResetObjectiveStats()
{
	if (!IsValid(ObjectiveStatsWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::ResetObjectiveStats() -> ObjectiveStatsWidget is not Valid!!!"));
		return;
	}
	ObjectiveStatsWidget->ResetStats();
}

void AMultiplayerFPSInGameHUD::ResetGameLeader()
{
	if (!IsValid(ObjectiveStatsWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::ResetGameLeader() -> ObjectiveStatsWidget is not Valid!!!"));
		return;
	}
	ObjectiveStatsWidget->ResetGameLeader();
}

void AMultiplayerFPSInGameHUD::SetOptionMenuVisibility(bool Visibility)
{
	if (!IsValid(InGameMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::SetOptionsMenuVisibility(bool Visibility) -> InGameMenuWidget is not Valid !!!"));
		return;
	}

	if (Visibility)
	{
		InGameMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMultiplayerFPSInGameHUD::SetBuyMenuVisibility(bool Visibility)
{
	if (!IsValid(BuyMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT(" AMultiplayerFPSInGameHUD::ToggleBuyMenu() -> BuyMenuWidget is not Valid !!!"));
		return;
	}
	if (Visibility)
	{
		BuyMenuWidget->SetFocus();
		BuyMenuWidget->SetKeyboardFocus();
		BuyMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BuyMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMultiplayerFPSInGameHUD::SetImageWeaponSlot(uint32 SlotIndex, UTexture2D* WeaponTexture2D)
{
	if (!IsValid(WeaponSlotsWidget))
	{
		UE_LOG(LogTemp, Error, TEXT(" AMultiplayerFPSInGameHUD::SelectWeaponSlot() -> WeaponSlotsWidget is not Valid !!!"));
		return;
	}
	WeaponSlotsWidget->SetImageWeaponSlot(SlotIndex, WeaponTexture2D);
}

void AMultiplayerFPSInGameHUD::SelectWeaponSlot(uint32 SlotIndex)
{
	if (!IsValid(WeaponSlotsWidget))
	{
		UE_LOG(LogTemp, Error, TEXT(" AMultiplayerFPSInGameHUD::SelectWeaponSlot() -> WeaponSlotsWidget is not Valid !!!"));
		return;
	}
	WeaponSlotsWidget->SelectWeaponSlot(SlotIndex);
}

void AMultiplayerFPSInGameHUD::ToggleLeaderBoardVisibility()
{
	if (!IsValid(LeaderBoardWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::ToggleLeaderBoardVisibility() -> LeaderBoardWidget is not Valid!!!"));
		return;
	}
	if (LeaderBoardWidget->IsVisible())
	{
		LeaderBoardWidget->RemoveFromParent();
	}
	else
	{
		UpdateLeaderBoardStats();
	}
}

void AMultiplayerFPSInGameHUD::UpdateLeaderBoardStats()
{
	if (!IsValid(LeaderBoardWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMultiplayerFPSInGameHUD::UpdateLeaderBoardStats() -> LeaderBoardWidget is not Valid!!!"));
		return;
	}

	LeaderBoardWidget->SetVisibility(ESlateVisibility::Hidden);
	LeaderBoardWidget->RemoveFromParent();
	LeaderBoardWidget = CreateWidget<ULeaderBoardWidget>(GetWorld(), LeaderBoardWidgetClass);
	if (!IsValid(LeaderBoardWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::UpdateLeaderBoardStats() -> LeaderBoardWidget is not Valid!!!"));
		return;
	}
	LeaderBoardWidget->AddToViewport();
}

void AMultiplayerFPSInGameHUD::UpdateLeaderBoard()
{
	if (!IsValid(LeaderBoardWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::UpdateLeaderBoard() -> LeaderBoardWidget is not Valid!!!"));
		return;
	}
	LeaderBoardWidget->UpdateLeaderBoard();
}

void AMultiplayerFPSInGameHUD::SetGlobalGameMessage(FString Message)
{
	if (!IsValid(GameTimeWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::SetGlobalGameMessage(FString Message) -> GameTimeWidget is not Valid!!!"));
		return;
	}
	GameTimeWidget->SetGlobalGameMessage(Message);
}

void AMultiplayerFPSInGameHUD::SelectWeapon(UBuyMenuWeaponUnitWidget* SelectedWeapon)
{
	if (!IsValid(BuyMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::SelectWeapon() -> BuyMenuWidget is not Valid !!!"));
		return;
	}
	BuyMenuWidget->SelectWeapon(SelectedWeapon);
}

void AMultiplayerFPSInGameHUD::ClearGlobalGameMessage()
{
	if (!IsValid(GameTimeWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::ClearGlobalGameMessage() -> GameTimeWidget is not Valid!!!"));
		return;
	}
	GameTimeWidget->ClearGlobalGameMessage();
}

void AMultiplayerFPSInGameHUD::GameEnded(const FString& Winner)
{
	if (!IsValid(LeaderBoardWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::GameEnded(ETeams WinnerTeam) -> LeaderBoardWidget is not Valid!!!"));
		return;
	}
	LeaderBoardWidget->RemoveFromViewport();

	if (!IsValid(BuyMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::GameEnded(ETeams WinnerTeam) -> BuyMenuWidget is not Valid!!!"));
		return;
	}
	BuyMenuWidget->RemoveFromViewport();

	if (!IsValid(InGameMenuWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::GameEnded(ETeams WinnerTeam) -> InGameMenuWidget is not Valid!!!"));
		return;
	}
	InGameMenuWidget->RemoveFromViewport();

	if (!IsValid(EndGameScreenWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("AMultiplayerFPSInGameHUD::GameEnded(ETeams WinnerTeam) -> EndGameScreenWidget is not Valid!!!"));
		return;
	}
	EndGameScreenWidget->SetWinnerTeam(Winner);
	EndGameScreenWidget->SetVisibility(ESlateVisibility::Visible);

}



