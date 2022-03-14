#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/ObjectiveStatsWidget.h"
#include "DM_ObjectiveStatsWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UDM_ObjectiveStatsWidget : public UObjectiveStatsWidget
{
	GENERATED_BODY()

public:
	UDM_ObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void ResetStats() override;

};
