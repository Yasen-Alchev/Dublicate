#include "HealthPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h"
#include "../Default/MultiplayerFPSCharacter.h"
#include "../Default/MultiplayerFPSHealthSystem.h"

AHealthPickup::AHealthPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthPickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_HealthPickup"));

	RootComponent = HealthPickupMesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetBoxExtent(FVector(75.0f, 60.0f, 60.0f));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::OnBeginOverlap);
}

void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPickup::OnBeginOverlap(class UPrimitiveComponent* HitComponent, 
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

		MultiplayerFPSPlayer->OnHealEvent.Broadcast(100.0f);

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

void AHealthPickup::ServerDestroyHealthPickup_Implementation()
{
	this->ClientDestroyHealthPickup();
}

void AHealthPickup::ClientDestroyHealthPickup_Implementation()
{
	this->Destroy();
}