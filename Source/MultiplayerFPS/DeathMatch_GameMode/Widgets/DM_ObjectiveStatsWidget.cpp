#include "DM_ObjectiveStatsWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UDM_ObjectiveStatsWidget::UDM_ObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

void UDM_ObjectiveStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDM_ObjectiveStatsWidget::ResetStats()
{
	if (IsValid(TXTBlock_RedScoreStat) && IsValid(TXTBlock_BlueScoreStat))
	{
		TXTBlock_RedScoreStat->SetText(FText::FromString(""));
		TXTBlock_BlueScoreStat->SetText(FText::FromString(""));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDM_ObjectiveStatsWidget::ResetStats() -> TXTBlock_RedScoreStat or  TXTBlock_BlueScoreStat is not Valid!!!"));
	}
}

