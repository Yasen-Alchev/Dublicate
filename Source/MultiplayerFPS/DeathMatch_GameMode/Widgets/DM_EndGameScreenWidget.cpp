#include "DM_EndGameScreenWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Math/Color.h"

UDM_EndGameScreenWidget::UDM_EndGameScreenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UDM_EndGameScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDM_EndGameScreenWidget::SetWinnerTeam(const FString& Winner)
{
	Super::SetWinnerTeam(Winner);
	if (IsValid(TXTBlock_EndGameWinnerText))
	{
		TXTBlock_EndGameWinnerText->SetText(FText::FromString("Winner: " + Winner));
		TXTBlock_EndGameWinnerText->SetColorAndOpacity(FLinearColor(FColor::Orange));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_EndGameScreenWidget::SetWinner(FName WinnerName) -> TXTBlock_EndGameWinnerText is not Valid !!!"));
	}
}





