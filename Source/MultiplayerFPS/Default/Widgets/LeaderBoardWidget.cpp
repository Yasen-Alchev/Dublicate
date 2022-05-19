// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderBoardWidget.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerState.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedCharacter.h"
#include "MultiplayerFPS/Default/TeamBasedClasses/TeamBasedPlayerState.h"
#include "Runtime/UMG/Public/UMG.h"
#include "WidgetUnits/LeaderBoardUnitWidget.h"
#include "WidgetUnits/ObjectiveLeaderBoardUnitWidget.h"

ULeaderBoardWidget::ULeaderBoardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void ULeaderBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerToWidgetMap = TMap<int32, int32>();

	GenerateLeaderBoard();
}

void ULeaderBoardWidget::GenerateLeaderBoard()
{
	InitLeaderBoardColumns();

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> World is not Valid !!!"));
		return;
	}

	if (!IsValid(VerticalBox))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> VerticalBox is not Valid !!!"));
		return;
	}
	if (!IsValid(LeaderBoardUnitRef))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> LeaderBoardUnitRef is not Valid !!!"));
		return;
	}
	if (!TSubclassOf<ULeaderBoardUnitWidget>(LeaderBoardUnitRef))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AMultiplayerFPSCharacter::StaticClass(), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> NO Actors of class AMultiplayerFPSCharacter Found !!!"));
		return;
	}

	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		AMultiplayerFPSCharacter* Character = Cast<AMultiplayerFPSCharacter>(FoundActors[i]);
		if (!IsValid(Character))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> Character is not Valid !!!"));
			return;
		}

		AMultiplayerFPSPlayerState* PlayerStateVar = Cast<AMultiplayerFPSPlayerState>(Character->GetPlayerState());
		if (!IsValid(PlayerStateVar))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> PlayerStateVar is not Valid !!!"));
			return;
		}

		ULeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<ULeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
		if (!IsValid(NewLeaderBoardUnit))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> NewLeaderBoardUnit is not Valid !!!"));
			return;
		}

		NewLeaderBoardUnit->SetPlayerName(PlayerStateVar->GetPlayerName());
		NewLeaderBoardUnit->SetPlayerKills(FString::FromInt(PlayerStateVar->GetPlayerKills()));
		NewLeaderBoardUnit->SetPlayerDeaths(FString::FromInt(PlayerStateVar->GetPlayerDeaths()));
		NewLeaderBoardUnit->SetPlayerScore(FString::FromInt(PlayerStateVar->GetPlayerScore()));

		if (TSubclassOf<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef) && TSubclassOf<ATeamBasedCharacter>(FoundActors[i]->GetClass()))
		{
			ATeamBasedPlayerState* TeamBasedPlayerState = Cast<ATeamBasedPlayerState>(PlayerStateVar);
			if (!IsValid(TeamBasedPlayerState))
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> TeamBasedPlayerState is not Valid !!!"));
				return;
			}
			UObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
			if (!IsValid(NewObjectLeaderBoardUnit))
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::GenerateLeaderBoard() -> NewObjectLeaderBoardUnit is not Valid !!!"));
				return;
			}
			NewObjectLeaderBoardUnit->SetPlayerObjectiveScore(FString::FromInt(TeamBasedPlayerState->GetCapturedFlagsCount()));
		}

		VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
		PlayerToWidgetMap.Add(PlayerStateVar->GetPlayerId(), VerticalBox->GetAllChildren().Num() - 1);
	}
}

void ULeaderBoardWidget::UpdateLeaderBoard()
{
	if (!IsValid(VerticalBox))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> VerticalBox is not Valid !!!"));
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMultiplayerFPSCharacter::StaticClass(), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> NO Actors of class AMultiplayerFPSCharacter Found !!!"));
		return;
	}

	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		AMultiplayerFPSCharacter* Character = Cast<AMultiplayerFPSCharacter>(FoundActors[i]);
		if (!IsValid(Character))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> Character is not Valid !!!"));
			return;
		}

		AMultiplayerFPSPlayerState* PlayerState= Cast<AMultiplayerFPSPlayerState>(Character->GetPlayerState());
		if (!IsValid(PlayerState))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> PlayerState is not Valid !!!"));
			return;
		}

		int32 PlayerID = PlayerState->GetPlayerId();
		int32 PlayerWidgetIndex = PlayerToWidgetMap[PlayerID];

		UWidget* ChildWidget = VerticalBox->GetChildAt(PlayerWidgetIndex);
		if (!IsValid(ChildWidget))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> ChildWidget is not Valid !!!"));
			return;
		}

		ULeaderBoardUnitWidget* LeaderBoardUnit = Cast<ULeaderBoardUnitWidget>(ChildWidget);
		if (!IsValid(LeaderBoardUnit))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> LeaderBoardUnit is not Valid !!!"));
			return;
		}

		LeaderBoardUnit->SetPlayerName(PlayerState->GetPlayerName());
		LeaderBoardUnit->SetPlayerKills(FString::FromInt(PlayerState->GetPlayerKills()));
		LeaderBoardUnit->SetPlayerDeaths(FString::FromInt(PlayerState->GetPlayerDeaths()));
		LeaderBoardUnit->SetPlayerScore(FString::FromInt(PlayerState->GetPlayerScore()));

		if (TSubclassOf<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef) && TSubclassOf<ATeamBasedCharacter>(FoundActors[i]->GetClass()))
		{
			ATeamBasedPlayerState* TeamBasedPlayerState = Cast<ATeamBasedPlayerState>(PlayerState);
			if (!IsValid(TeamBasedPlayerState))
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> TeamBasedPlayerState is not Valid !!!"));
				return;
			}
			UObjectiveLeaderBoardUnitWidget* ObjectLeaderBoardUnit = Cast<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnit);
			if (!IsValid(ObjectLeaderBoardUnit))
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> ObjectLeaderBoardUnit is not Valid !!!"));
				return;
			}
			ObjectLeaderBoardUnit->SetPlayerObjectiveScore(FString::FromInt(TeamBasedPlayerState->GetCapturedFlagsCount()));
		}
	}


	/*
	 for(auto ax : PlayerToWidgetMap)
	{
		UE_LOG(LogTemp, Error, TEXT("Key = %d                 Value = %d"), ax.Key, ax.Value);
	}

	const int32* PlayerWidgetIndex = PlayerToWidgetMap.Find(PlayerStateVar->GetPlayerId());

	UE_LOG(LogTemp, Error, TEXT("PlayerWidgetIndex = %d"), PlayerWidgetIndex);

	if (VerticalBox->GetAllChildren().Num() <= *PlayerWidgetIndex)
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> PlayerWidgetIndex Out Of Bound !!!"));
		return;
	}

	FString PlayerNameVar = PlayerStateVar->GetPlayerName();
	UWidget* ChildWidget = VerticalBox->GetChildAt(*PlayerWidgetIndex);

	if (!IsValid(ChildWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> ChildWidget is not Valid !!!"));
		return;
	}

	ULeaderBoardUnitWidget* Child = Cast<ULeaderBoardUnitWidget>(ChildWidget);

	if (!IsValid(Child))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> Child is not Valid !!!"));
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("PlayerStateVar->GetPlayerName:   %s                ChildWidget->PlayerName:          %s"), *PlayerStateVar->GetPlayerName(), *Child->GetPlayerName());*/



	/*for(auto Child: ChildWidgets)
	{
		ULeaderBoardUnitWidget* LeaderBoardUnit = Cast<ULeaderBoardUnitWidget>(Child);
		if (!IsValid(LeaderBoardUnit))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> LeaderBoardUnit is not Valid !!!"));
			return;
		}
		if(PlayerNameVar.Equals( LeaderBoardUnit->GetPlayerName()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Red, FString::Printf(TEXT("%s     FOUNDED!!!!"), *PlayerNameVar));

			LeaderBoardUnit->SetPlayerKills(FString::FromInt(PlayerStateVar->GetPlayerKills()));
			LeaderBoardUnit->SetPlayerDeaths(FString::FromInt(PlayerStateVar->GetPlayerDeaths()));
			LeaderBoardUnit->SetPlayerScore(FString::FromInt(PlayerStateVar->GetPlayerScore()));

			if (TSubclassOf<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef) && TSubclassOf<ATeamBasedPlayerState>(PlayerStateVar->GetClass()))
			{
				ATeamBasedPlayerState* TeamBasedPlayerState = Cast<ATeamBasedPlayerState>(PlayerStateVar);
				if (!IsValid(TeamBasedPlayerState))
				{
					UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> TeamBasedPlayerState is not Valid !!!"));
					return;
				}
				UObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnit);
				if (!IsValid(NewObjectLeaderBoardUnit))
				{
					UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::UpdateLeaderBoard() -> LeaderBoardUnit is not Valid !!!"));
					return;
				}

				NewObjectLeaderBoardUnit->SetPlayerName(TeamBasedPlayerState->GetPlayerName());
				NewObjectLeaderBoardUnit->SetPlayerObjectiveScore(FString::FromInt(TeamBasedPlayerState->GetCapturedFlagsCount()));

			}

		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("UpdateLeaderBoard -> PlayerName: %s"), *PlayerNameVar));
	*/
}

void ULeaderBoardWidget::InitLeaderBoardColumns()
{
	if (!IsValid(VerticalBox))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> VerticalBox is not Valid !!!"));
		return;
	}

	if (!IsValid(LeaderBoardUnitRef))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> LeaderBoardUnitRef is not Valid !!!"));
		return;
	}

	if (!TSubclassOf<ULeaderBoardUnitWidget>(LeaderBoardUnitRef))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
		return;
	}

	ULeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<ULeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
	if (!IsValid(NewLeaderBoardUnit))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> NewLeaderBoardUnit is not Valid !!!"));
		return;
	}

	NewLeaderBoardUnit->SetPlayerName("Player");
	NewLeaderBoardUnit->SetPlayerKills("Kills");
	NewLeaderBoardUnit->SetPlayerDeaths("Deaths");
	NewLeaderBoardUnit->SetPlayerScore("Score");

	if (TSubclassOf<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef))
	{
		UObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
		if (!IsValid(NewObjectLeaderBoardUnit))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> NewObjectLeaderBoardUnit is not Valid !!!"));
			return;
		}
		NewObjectLeaderBoardUnit->SetPlayerObjectiveScore("Flags Captured");
	}

	VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
}
