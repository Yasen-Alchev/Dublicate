// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameScreenWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Math/Color.h"

UEndGameScreenWidget::UEndGameScreenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

void UEndGameScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(IsValid(Button_EndGame))
	{
		Button_EndGame->OnClicked.AddDynamic(this, &UEndGameScreenWidget::QuitButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UEndGameScreenWidget::NativeConstruct() -> Button_EndGame is not Valid !!!"));
	}
	if (IsValid(Button_ReturnToMainMenu))
	{
		Button_ReturnToMainMenu->OnClicked.AddDynamic(this, &UEndGameScreenWidget::ReturnToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UEndGameScreenWidget::NativeConstruct() -> Button_ReturnToMainMenu is not Valid !!!"));
	}
}

void UEndGameScreenWidget::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UEndGameScreenWidget::ReturnToMainMenu()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		World->GetFirstPlayerController()->ClientTravel("/Game/ThirdPersonCPP/Maps/MainMenuLevel.MainMenuLevel", TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UEndGameScreenWidget::ReturnToMainMenu() -> World is not Valid !!!"));
	}
}

void UEndGameScreenWidget::SetWinnerTeam(ETeams WinnerTeam)
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
		UE_LOG(LogTemp, Error, TEXT("UEndGameScreenWidget::SetWinnerTeam(ETeams WinnerTeam) -> TXTBlock_EndGameWinnerText is not Valid!!!"));
	}
}

FReply UEndGameScreenWidget::NativeOnKeyDown(const FGeometry& MovieSceneBlends, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(MovieSceneBlends, InKeyEvent);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("CALLLED YAYA")));
	return Super::NativeOnKeyDown(MovieSceneBlends, InKeyEvent);

}




