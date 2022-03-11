#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerFPS/Default/Widgets/LeaderBoardWidget.h"
#include "CTF_LeaderBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERFPS_API UCTF_LeaderBoardWidget : public ULeaderBoardWidget
{
	GENERATED_BODY()

public:

	UCTF_LeaderBoardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
