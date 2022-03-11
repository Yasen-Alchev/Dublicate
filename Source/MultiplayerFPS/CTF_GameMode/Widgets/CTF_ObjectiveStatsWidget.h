#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/ObjectiveStatsWidget.h"
#include "CTF_ObjectiveStatsWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UCTF_ObjectiveStatsWidget : public UObjectiveStatsWidget
{
	GENERATED_BODY()

public:
	UCTF_ObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
