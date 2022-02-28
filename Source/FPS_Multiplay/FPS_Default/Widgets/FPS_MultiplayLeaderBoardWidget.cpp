// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayLeaderBoardWidget.h"
#include "FPS_MultiplayLeaderBoardUnitWidget.h"
#include "FPS_MultiplayObjectiveLeaderBoardUnitWidget.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayCharacter.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayPlayerState.h"
#include "FPS_Multiplay/FPS_Default/FPS_MultiplayTeamBasedCharacter.h"
#include "Runtime/UMG/Public/UMG.h"

UFPS_MultiplayLeaderBoardWidget::UFPS_MultiplayLeaderBoardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UFPS_MultiplayLeaderBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GenerateLeaderBoard();
}

void UFPS_MultiplayLeaderBoardWidget::GenerateLeaderBoard()
{
	InitLeaderBoardColumns();

	if (IsValid(VerticalBox))
	{
		TArray<AActor*> FoundActors;	
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPS_MultiplayCharacter::StaticClass(), FoundActors);

		if (FoundActors.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NO Actors of class AFPS_MultiplayCharacter Found !!!"));
		}

		for (AActor* Player : FoundActors)
		{
			AFPS_MultiplayCharacter* Character = Cast<AFPS_MultiplayCharacter>(Player);
			if (IsValid(Character))
			{
				if (IsValid(LeaderBoardUnitRef))
				{
					if (TSubclassOf<UFPS_MultiplayLeaderBoardUnitWidget>(LeaderBoardUnitRef))
					{
						AFPS_MultiplayPlayerState* PlayerStateVar = Cast<AFPS_MultiplayPlayerState>(Character->GetPlayerState());
						if (IsValid(PlayerStateVar))
						{
							UFPS_MultiplayLeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<UFPS_MultiplayLeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
							if (IsValid(NewLeaderBoardUnit))
							{
								NewLeaderBoardUnit->SetPlayerName(Character->PlayerName);
								NewLeaderBoardUnit->SetPlayerKills(FString::FromInt(PlayerStateVar->getPlayerKills()));
								NewLeaderBoardUnit->SetPlayerDeaths(FString::FromInt(PlayerStateVar->getPlayerDeaths()));
								NewLeaderBoardUnit->SetPlayerScore(FString::FromInt(PlayerStateVar->getPlayerScore()));
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NewLeaderBoardUnit is not Valid !!!"));
							}

							if (TSubclassOf<UFPS_MultiplayObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef) && TSubclassOf<AFPS_MultiplayTeamBasedCharacter>(Player->GetClass()))
							{
								AFPS_MultiplayTeamBasedCharacter* TeamCharacter = Cast<AFPS_MultiplayTeamBasedCharacter>(Player);
								if (IsValid(TeamCharacter))
								{
									UFPS_MultiplayObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UFPS_MultiplayObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
									if (IsValid(NewObjectLeaderBoardUnit))
									{
										NewObjectLeaderBoardUnit->SetPlayerObjectiveScore(FString::FromInt(PlayerStateVar->getCapturedFlagsCount()));
									}
									else
									{
										UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> NewObjectLeaderBoardUnit is not Valid !!!"));
									}
								}
								else
								{
									UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> TeamCharacter is not Valid !!!"));
								}
							}
							VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> PlayerStateVar is not Valid !!!"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> LeaderBoardUnitRef is not Valid !!!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> Character is not Valid !!!"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::CreateNewLeaderBoardUnit(AActor* Owner) -> VerticalBox is not Valid !!!"));
	}
}

void UFPS_MultiplayLeaderBoardWidget::InitLeaderBoardColumns()
{
	if (IsValid(VerticalBox))
	{
		if (IsValid(LeaderBoardUnitRef))
		{
			if (TSubclassOf<UFPS_MultiplayLeaderBoardUnitWidget>(LeaderBoardUnitRef))
			{
				UFPS_MultiplayLeaderBoardUnitWidget* NewLeaderBoardUnit = CreateWidget<UFPS_MultiplayLeaderBoardUnitWidget>(this, LeaderBoardUnitRef);
				if (IsValid(NewLeaderBoardUnit))
				{
					NewLeaderBoardUnit->SetPlayerName("Player");
					NewLeaderBoardUnit->SetPlayerKills("Kills");
					NewLeaderBoardUnit->SetPlayerDeaths("Deaths");
					NewLeaderBoardUnit->SetPlayerScore("Score");

					if (TSubclassOf<UFPS_MultiplayObjectiveLeaderBoardUnitWidget>(LeaderBoardUnitRef))
					{
						UFPS_MultiplayObjectiveLeaderBoardUnitWidget* NewObjectLeaderBoardUnit = Cast<UFPS_MultiplayObjectiveLeaderBoardUnitWidget>(NewLeaderBoardUnit);
						if (IsValid(NewObjectLeaderBoardUnit))
						{
							NewObjectLeaderBoardUnit->SetPlayerObjectiveScore("Flags Captured");
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::InitLeaderBoardColumns() -> NewObjectLeaderBoardUnit is not Valid !!!"));
						}
					}
					VerticalBox->AddChildToVerticalBox(NewLeaderBoardUnit);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::InitLeaderBoardColumns() -> NewLeaderBoardUnit is not Valid !!!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::InitLeaderBoardColumns() -> Unsupported TSubclassOf<T>(LeaderBoardUnitRef) !!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::InitLeaderBoardColumns() -> LeaderBoardUnitRef is not Valid !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayLeaderBoardWidget::InitLeaderBoardColumns() -> VerticalBox is not Valid !!!"));
	}
}
