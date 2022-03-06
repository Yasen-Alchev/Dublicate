// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerFPSLeaderBoardWidget.h"
#include "MultiplayerFPSLeaderBoardUnitWidget.h"
#include "MultiplayerFPSObjectiveLeaderBoardUnitWidget.h"
#include "MultiplayerFPS/Default/MultiplayerFPSPlayerState.h"
#include "MultiplayerFPS/Default/MultiplayerFPSTeamBasedCharacter.h"
#include "Runtime/UMG/Public/UMG.h"

UMultiplayerFPSLeaderBoardWidget::UMultiplayerFPSLeaderBoardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UMultiplayerFPSLeaderBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GenerateLeaderBoard();
}

void UMultiplayerFPSLeaderBoardWidget::GenerateLeaderBoard()
{
	InitLeaderBoardColumns();

	if (IsValid(VerticalBox))
	{
		TArray<AActor*> FoundActors;	
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMultiplayerFPSCharacter::StaticClass(), FoundActors);

		if (FoundActors.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NO Actors of class AMultiplayerFPSCharacter Found !!!"));
		}

		for (AActor* Player : FoundActors)
		{
			AMultiplayerFPSCharacter* Character = Cast<AMultiplayerFPSCharacter>(Player);
			if (IsValid(Character))
			{
				if (IsValid(LeaderBoardUnitRef))
				{
					if (TSubclassOf<UMultiplayerFPSLeaderBoardUnitWidget>(LeaderBoardUnitRef))
					{
						AMultiplayerFPSPlayerState* PlayerStateVar = Cast<AMultiplayerFPSPlayerState>(Character->GetPlayerState());
						if (IsValid(PlayerStateVar))
						{
							UMultiplayerFPSLeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<UMultiplayerFPSLeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
							if (IsValid(NewLeaderBoardUnit))
							{
								NewLeaderBoardUnit->SetPlayerName(Character->PlayerName);
								NewLeaderBoardUnit->SetPlayerKills(FString::FromInt(PlayerStateVar->getPlayerKills()));
								NewLeaderBoardUnit->SetPlayerDeaths(FString::FromInt(PlayerStateVar->getPlayerDeaths()));
								NewLeaderBoardUnit->SetPlayerScore(FString::FromInt(PlayerStateVar->getPlayerScore()));
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NewLeaderBoardUnit is not Valid !!!"));
							}

							if (TSubclassOf<UMultiplayerFPSObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef) && TSubclassOf<AMultiplayerFPSTeamBasedCharacter>(Player->GetClass()))
							{
								AMultiplayerFPSTeamBasedCharacter* TeamCharacter = Cast<AMultiplayerFPSTeamBasedCharacter>(Player);
								if (IsValid(TeamCharacter))
								{
									UMultiplayerFPSObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UMultiplayerFPSObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
									if (IsValid(NewObjectLeaderBoardUnit))
									{
										NewObjectLeaderBoardUnit->SetPlayerObjectiveScore(FString::FromInt(PlayerStateVar->getCapturedFlagsCount()));
									}
									else
									{
										UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NewObjectLeaderBoardUnit is not Valid !!!"));
									}
								}
								else
								{
									UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> TeamCharacter is not Valid !!!"));
								}
							}
							VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> PlayerStateVar is not Valid !!!"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> LeaderBoardUnitRef is not Valid !!!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> Character is not Valid !!!"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> VerticalBox is not Valid !!!"));
	}
}

void UMultiplayerFPSLeaderBoardWidget::InitLeaderBoardColumns()
{
	if (IsValid(VerticalBox))
	{
		if (IsValid(LeaderBoardUnitRef))
		{
			if (TSubclassOf<UMultiplayerFPSLeaderBoardUnitWidget>(LeaderBoardUnitRef))
			{
				UMultiplayerFPSLeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<UMultiplayerFPSLeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
				if (IsValid(NewLeaderBoardUnit))
				{
					NewLeaderBoardUnit->SetPlayerName("Player");
					NewLeaderBoardUnit->SetPlayerKills("Kills");
					NewLeaderBoardUnit->SetPlayerDeaths("Deaths");
					NewLeaderBoardUnit->SetPlayerScore("Score");

					if (TSubclassOf<UMultiplayerFPSObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef))
					{
						UMultiplayerFPSObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UMultiplayerFPSObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
						if (IsValid(NewObjectLeaderBoardUnit))
						{
							NewObjectLeaderBoardUnit->SetPlayerObjectiveScore("Flags Captured");
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::InitLeaderBoardColumns() -> NewObjectLeaderBoardUnit is not Valid !!!"));
						}
					}
					VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::InitLeaderBoardColumns() -> NewLeaderBoardUnit is not Valid !!!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::InitLeaderBoardColumns() -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::InitLeaderBoardColumns() -> LeaderBoardUnitRef is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSLeaderBoardWidget::InitLeaderBoardColumns() -> VerticalBox is not Valid !!!"));
	}
}
