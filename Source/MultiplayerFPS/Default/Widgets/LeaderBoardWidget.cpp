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

	GenerateLeaderBoard();
}

void ULeaderBoardWidget::GenerateLeaderBoard()
{
	InitLeaderBoardColumns();

	if (!IsValid(VerticalBox))
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> VerticalBox is not Valid !!!"));
		return;
	}
	TArray<AActor*> FoundActors;	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMultiplayerFPSCharacter::StaticClass(), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NO Actors of class AMultiplayerFPSCharacter Found !!!"));
		return;
	}

	for (AActor* Player : FoundActors)
	{
		AMultiplayerFPSCharacter* Character = Cast<AMultiplayerFPSCharacter>(Player);
		if (!IsValid(Character))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> Character is not Valid !!!"));
			return;
		}
		if (!IsValid(LeaderBoardUnitRef))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> LeaderBoardUnitRef is not Valid !!!"));
			return;
		}
		if (!TSubclassOf<ULeaderBoardUnitWidget>(LeaderBoardUnitRef))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
			return;
		}
		AMultiplayerFPSPlayerState* PlayerStateVar = Cast<AMultiplayerFPSPlayerState>(Character->GetPlayerState());
		if (!IsValid(PlayerStateVar))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> PlayerStateVar is not Valid !!!"));
			return;
		}

		ULeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<ULeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
		if (!IsValid(NewLeaderBoardUnit))
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NewLeaderBoardUnit is not Valid !!!"));
			return;
		}

		NewLeaderBoardUnit->SetPlayerName(PlayerStateVar->GetPlayerName());
		NewLeaderBoardUnit->SetPlayerKills(FString::FromInt(PlayerStateVar->getPlayerKills()));
		NewLeaderBoardUnit->SetPlayerDeaths(FString::FromInt(PlayerStateVar->getPlayerDeaths()));
		NewLeaderBoardUnit->SetPlayerScore(FString::FromInt(PlayerStateVar->getPlayerScore()));

		if (TSubclassOf<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef) && TSubclassOf<ATeamBasedCharacter>(Player->GetClass()))
		{
			ATeamBasedCharacter* TeamCharacter = Cast<ATeamBasedCharacter>(Player);
			if (!IsValid(TeamCharacter))
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> TeamCharacter is not Valid !!!"));
				return;
			}
			ATeamBasedPlayerState* TeamBasedPlayerState = Cast<ATeamBasedPlayerState>(PlayerStateVar);
			if (!IsValid(TeamBasedPlayerState))
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> TeamBasedPlayerState is not Valid !!!"));
				return;
			}
			UObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
			if (!IsValid(NewObjectLeaderBoardUnit))
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NewObjectLeaderBoardUnit is not Valid !!!"));
				return;
			}

			NewObjectLeaderBoardUnit->SetPlayerName(TeamBasedPlayerState->GetPlayerName());
			NewObjectLeaderBoardUnit->SetPlayerObjectiveScore(FString::FromInt(TeamBasedPlayerState->getCapturedFlagsCount()));
					
		}
		VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
	}
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
