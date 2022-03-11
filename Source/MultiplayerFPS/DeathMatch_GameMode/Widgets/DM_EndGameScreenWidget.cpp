#include "DM_EndGameScreenWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Math/Color.h"

UDM_EndGameScreenWidget::UDM_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UDM_EndGameScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(Button_EndGame))
	{
		Button_EndGame->OnClicked.AddDynamic(this, &UDM_EndGameScreenWidget::QuitButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_EndGameScreenWidget::NativeConstruct() -> Button_EndGame is not Valid !!!"));
	}
	if (IsValid(Button_ReturnToMainMenu))
	{
		Button_ReturnToMainMenu->OnClicked.AddDynamic(this, &UDM_EndGameScreenWidget::ReturnToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_EndGameScreenWidget::NativeConstruct() -> Button_ReturnToMainMenu is not Valid !!!"));
	}
}

void UDM_EndGameScreenWidget::QuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UDM_EndGameScreenWidget::ReturnToMainMenu()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		World->GetFirstPlayerController()->ClientTravel("/Game/ThirdPersonCPP/Maps/MainMenuLevel.MainMenuLevel", TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_EndGameScreenWidget::ReturnToMainMenu() -> World is not Valid !!!"));
	}
}

void UDM_EndGameScreenWidget::SetWinner(const FString& WinnerName)
{
	if (IsValid(TXTBlock_EndGameWinnerText))
	{
		TXTBlock_EndGameWinnerText->SetText(FText::FromString("Winner: " + WinnerName));
		TXTBlock_EndGameWinnerText->SetColorAndOpacity(FLinearColor(FColor::Orange));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_EndGameScreenWidget::SetWinner(FName WinnerName) -> TXTBlock_EndGameWinnerText is not Valid !!!"));
	}
}

FReply UDM_EndGameScreenWidget::NativeOnKeyDown(const FGeometry& MovieSceneBlends, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyDown(MovieSceneBlends, InKeyEvent);
}




