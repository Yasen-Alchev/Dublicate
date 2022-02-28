// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_MultiplayEndGameScreenWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Math/Color.h"

UFPS_MultiplayEndGameScreenWidget::UFPS_MultiplayEndGameScreenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

void UFPS_MultiplayEndGameScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(IsValid(Button_EndGame))
	{
		Button_EndGame->OnClicked.AddDynamic(this, &UFPS_MultiplayEndGameScreenWidget::QuitButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayEndGameScreenWidget::NativeConstruct() -> Button_EndGame is not Valid !!!"));
	}
	if (IsValid(Button_ReturnToMainMenu))
	{
		Button_ReturnToMainMenu->OnClicked.AddDynamic(this, &UFPS_MultiplayEndGameScreenWidget::ReturnToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayEndGameScreenWidget::NativeConstruct() -> Button_ReturnToMainMenu is not Valid !!!"));
	}
}

void UFPS_MultiplayEndGameScreenWidget::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UFPS_MultiplayEndGameScreenWidget::ReturnToMainMenu()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		World->GetFirstPlayerController()->ClientTravel("/Game/ThirdPersonCPP/Maps/MainMenuLevel.MainMenuLevel", TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayEndGameScreenWidget::ReturnToMainMenu() -> World is not Valid !!!"));
	}
}

void UFPS_MultiplayEndGameScreenWidget::SetWinnerTeam(EFPS_Teams WinnerTeam)
{
	if(IsValid(TXTBlock_EndGameWinnerText))
	{
		switch (WinnerTeam)
		{
			case TEAM_BLUE:
				TXTBlock_EndGameWinnerText->SetText(FText::FromString("BLUE TEAM WINS!"));
				TXTBlock_EndGameWinnerText->SetColorAndOpacity(FLinearColor(FColor::Blue));
				break;
			case TEAM_RED:
				TXTBlock_EndGameWinnerText->SetText(FText::FromString("RED TEAM WINS!"));
				TXTBlock_EndGameWinnerText->SetColorAndOpacity(FLinearColor(FColor::Red));
				break;
			case TEAM_NONE:
				TXTBlock_EndGameWinnerText->SetText(FText::FromString("DRAW"));
				TXTBlock_EndGameWinnerText->SetColorAndOpacity(FLinearColor(FColor::Orange));
				break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UFPS_MultiplayEndGameScreenWidget::SetWinnerTeam(EFPS_Teams WinnerTeam) -> TXTBlock_EndGameWinnerText is not Valid!!!"));
	}
}

FReply UFPS_MultiplayEndGameScreenWidget::NativeOnKeyDown(const FGeometry& MovieSceneBlends, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(MovieSceneBlends, InKeyEvent);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("CALLLED YAYA")));
	return Super::NativeOnKeyDown(MovieSceneBlends, InKeyEvent);

}




