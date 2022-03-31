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

	if (IsValid(VerticalBox))
	{
		TArray<AActor*> FoundActors;	
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMultiplayerFPSCharacter::StaticClass(), FoundActors);

		if (FoundActors.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NO Actors of class AMultiplayerFPSCharacter Found !!!"));
		}

		for (AActor* Player : FoundActors)
		{
			AMultiplayerFPSCharacter* Character = Cast<AMultiplayerFPSCharacter>(Player);
			if (IsValid(Character))
			{
				if (IsValid(LeaderBoardUnitRef))
				{
					if (TSubclassOf<ULeaderBoardUnitWidget>(LeaderBoardUnitRef))
					{
						AMultiplayerFPSPlayerState* PlayerStateVar = Cast<AMultiplayerFPSPlayerState>(Character->GetPlayerState());
						if (IsValid(PlayerStateVar))
						{
							ULeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<ULeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
							if (IsValid(NewLeaderBoardUnit))
							{
								NewLeaderBoardUnit->SetPlayerName(Character->PlayerName);
								NewLeaderBoardUnit->SetPlayerKills(FString::FromInt(PlayerStateVar->getPlayerKills()));
								NewLeaderBoardUnit->SetPlayerDeaths(FString::FromInt(PlayerStateVar->getPlayerDeaths()));
								NewLeaderBoardUnit->SetPlayerScore(FString::FromInt(PlayerStateVar->getPlayerScore()));
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NewLeaderBoardUnit is not Valid !!!"));
							}

							if (TSubclassOf<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef) && TSubclassOf<ATeamBasedCharacter>(Player->GetClass()))
							{
								ATeamBasedCharacter* TeamCharacter = Cast<ATeamBasedCharacter>(Player);
								if (IsValid(TeamCharacter))
								{
									ATeamBasedPlayerState* TeamBasedPlayerState = Cast<ATeamBasedPlayerState>(PlayerStateVar);
									if (IsValid(TeamBasedPlayerState))
									{
										UObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
										if (IsValid(NewObjectLeaderBoardUnit))
										{
											NewObjectLeaderBoardUnit->SetPlayerName(TeamCharacter->PlayerName);
											NewObjectLeaderBoardUnit->SetPlayerObjectiveScore(FString::FromInt(TeamBasedPlayerState->getCapturedFlagsCount()));
										}
										else
										{
											UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NewObjectLeaderBoardUnit is not Valid !!!"));
										}
									}
									else
									{
										UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> TeamBasedPlayerState is not Valid !!!"));
									}
								}
								else
								{
									UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> TeamCharacter is not Valid !!!"));
								}
							}
							VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> PlayerStateVar is not Valid !!!"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> LeaderBoardUnitRef is not Valid !!!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> Character is not Valid !!!"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> VerticalBox is not Valid !!!"));
	}
}

void ULeaderBoardWidget::InitLeaderBoardColumns()
{
	if (IsValid(VerticalBox))
	{
		if (IsValid(LeaderBoardUnitRef))
		{
			if (TSubclassOf<ULeaderBoardUnitWidget>(LeaderBoardUnitRef))
			{
				ULeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<ULeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
				if (IsValid(NewLeaderBoardUnit))
				{
					NewLeaderBoardUnit->SetPlayerName("Player");
					NewLeaderBoardUnit->SetPlayerKills("Kills");
					NewLeaderBoardUnit->SetPlayerDeaths("Deaths");
					NewLeaderBoardUnit->SetPlayerScore("Score");

					if (TSubclassOf<UObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef))
					{
						UObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
						if (IsValid(NewObjectLeaderBoardUnit))	
						{
							NewObjectLeaderBoardUnit->SetPlayerObjectiveScore("Flags Captured");
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> NewObjectLeaderBoardUnit is not Valid !!!"));
						}
					}
					VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> NewLeaderBoardUnit is not Valid !!!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> LeaderBoardUnitRef is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ULeaderBoardWidget::InitLeaderBoardColumns() -> VerticalBox is not Valid !!!"));
	}
}
