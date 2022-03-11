#include "CQ_Character.h"

#include "MultiplayerFPS/Default/MultiplayerFPSGameInstance.h"
#include "Net/UnrealNetwork.h"

ACQ_Character::ACQ_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bIsInOptionsMenu = false;

	myMaterial = nullptr;
}


void ACQ_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ACQ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
