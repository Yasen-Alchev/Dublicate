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
	HealthPickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	RootComponent = HealthPickupMesh;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetBoxExtent(FVector(75.0f, 60.0f, 60.0f));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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

		MultiplayerFPSPlayer->HealthSystem->Heal(100);
		MultiplayerFPSPlayer->HealthSystem->StartShieldRecharge();

		UE_LOG(LogTemp, Warning, TEXT("Player Health Restored!"));

		this->DestroyHealthPickup();
	}
}

void AHealthPickup::DestroyHealthPickup()
{
	this->Destroy();
}