#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DM_ObjectiveStatsWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UDM_ObjectiveStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDM_ObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateStats();
	void ResetStats();
};
