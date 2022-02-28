// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS_Teams.h"
#include "GameFramework/Actor.h"
#include "FPS_Flag.generated.h"

UCLASS()
class FPS_MULTIPLAY_API AFPS_Flag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPS_Flag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
		TEnumAsByte<EFPS_Teams> FlagTeam;

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
