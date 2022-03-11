#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DM_LeaderBoardWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UDM_LeaderBoardWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UDM_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateStats(int Kills, int Deaths);
	void ResetStats();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTBlock_Kills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTBlock_Deaths;

};
