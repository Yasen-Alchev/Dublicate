#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/LeaderBoardWidget.h"
#include "DM_LeaderBoardWidget.generated.h"

UCLASS()
class MULTIPLAYERFPS_API UDM_LeaderBoardWidget : public ULeaderBoardWidget
{
	GENERATED_BODY()

public:

	UDM_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

};
