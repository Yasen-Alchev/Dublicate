#include "MultiplayerFPSPlayerState.h"

#include "Net/UnrealNetwork.h"


AMultiplayerFPSPlayerState::AMultiplayerFPSPlayerState()
{
	bReplicates = true;
	
	Kills = 0;
	Deaths = 0;
	PlayerScore = 0;
}

void AMultiplayerFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerFPSPlayerState, Kills);
	DOREPLIFETIME(AMultiplayerFPSPlayerState, Deaths);
	DOREPLIFETIME(AMultiplayerFPSPlayerState, PlayerScore);
}

void AMultiplayerFPSPlayerState::ServerHasDied_Implementation()
{
	++this->Deaths;
}


void AMultiplayerFPSPlayerState::ServerHasKilledPlayer_Implementation()
{
	++this->Kills;
	PlayerScore += 5;
}
