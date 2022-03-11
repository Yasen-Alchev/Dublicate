#pragma once

#include "CoreMinimal.h"
#include "Teams.h"
#include "GameFramework/Actor.h"
#include "Flag.generated.h"

UCLASS()
class MULTIPLAYERFPS_API AFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlag();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		TEnumAsByte<ETeams> FlagTeam;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
		void ServerRPCFlagOverlappedLogic(AActor* OtherActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Respawn")
		bool bIsDropped;

private:
	UPROPERTY()
		FTimerHandle RespawnHandle;

	UPROPERTY()
		int RespawnTime;

	UFUNCTION(Server, Reliable)
		void ServerRPCRespawnFlag();
};
