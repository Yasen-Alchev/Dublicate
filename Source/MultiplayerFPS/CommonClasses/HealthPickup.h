#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HealthPickup.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealthPickup();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* HealthPickupMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UBoxComponent* BoxCollision;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* HitComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerDestroyHealthPickup();

	UFUNCTION(Client, Reliable)
	void ClientDestroyHealthPickup();
};
