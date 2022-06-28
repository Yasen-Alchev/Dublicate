void AHealthPickup::OnBeginOverlap(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		AMultiplayerFPSCharacter* MultiplayerFPSPlayer = Cast<AMultiplayerFPSCharacter>(OtherActor);
		if (!IsValid(MultiplayerFPSPlayer))
		{
			UE_LOG(LogTemp, Error, TEXT("AHealthPickup::OnBeginOverlap !IsValid(MultiplayerFPSPlayer)"));
			return;
		}

		if (MultiplayerFPSPlayer->HealthSystem->GetCurrentHealth() == MultiplayerFPSPlayer->HealthSystem->GetMaxHealth())
		{
			return;
		}

		MultiplayerFPSPlayer->OnHealEvent.Broadcast(100.0f, this);

		UE_LOG(LogTemp, Warning, TEXT("Player Health Restored!"));

		if (MultiplayerFPSPlayer->HasAuthority())
		{
			this->ServerDestroyHealthPickup();
		}
		this->ClientDestroyHealthPickup();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AHealthPickup::OnBeginOverlap OtherActor->ActorHasTag(\"Player\")"));
	}
}