#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/ObjectiveStatsWidget.h"
#include "CQ_ObjectiveStatsWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UCQ_ObjectiveStatsWidget : public UObjectiveStatsWidget
{
	GENERATED_BODY()

public:
	UCQ_ObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

};
