#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MultiplayerFPSFirearm.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UBoxComponent;
class UAnimMontage;

#define ECC_Hitscan ECC_GameTraceChannel1

UENUM()
enum class EFireMode : uint8
{
	Semi UMETA(DisplayName = "Semi-Auto"),
	Burst UMETA(DisplayName = "Burst"),
	Auto UMETA(DisplayName = "Auto"),
};

UCLASS()
class MULTIPLAYERFPS_API AMultiplayerFPSFirearm : public AActor
{
	GENERATED_BODY()
	
public:	
	AMultiplayerFPSFirearm();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* GunMuzzleLocation;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, Category = Stats)
	float Damage;

	UPROPERTY(EditAnywhere, Category = Stats)
	int32 MaxMagazineCapacity;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugTrace;

	UPROPERTY(EditAnywhere, Category = Modes)
	int32 BurstCount;

	UPROPERTY(EditAnywhere, Category = Modes)
	float BurstFiringInterval;

	UPROPERTY(EditAnywhere, Category = Stats)
	float ReloadTime;

	UPROPERTY(EditAnywhere, Category = "Fire Modes")
	TArray<EFireMode> FireModesArray;

	UPROPERTY(EditAnywhere, Category = "Fire Modes")
	TArray<bool> bIsHeldArray;

	UPROPERTY(EditAnywhere, Category = "Fire Modes")
	TArray<float> HeldFiringIntervalsArray;

	UPROPERTY(EditAnywhere, Category = Zoom)
	float ZoomFOV;

private:

	UPROPERTY(Replicated)
	int32 BurstsFired;

	UPROPERTY()
	EFireMode CurrentFireMode;

	UPROPERTY(Replicated)
	int32 CurrentMagazineCapacity;

	UPROPERTY(Replicated)
	bool bIsFiring;

	UPROPERTY()
	FTimerHandle HeldFiringIntervalTimer;

	UPROPERTY()
	FTimerHandle BurstFiringIntervalTimer;

	UPROPERTY()
	FTimerHandle AllowFiringTimer;

	UFUNCTION()
	void Fire();

	UFUNCTION(Server, Reliable)
	void ServerFire();

	UFUNCTION()
	void BurstFire();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool ShouldReloadFirearm()
	{
		return (this->CurrentMagazineCapacity != this->MaxMagazineCapacity);
	}

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();

	UFUNCTION()
	void SwitchFireMode();

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void Zoom();

	UFUNCTION()
	void ZoomOut();
};
