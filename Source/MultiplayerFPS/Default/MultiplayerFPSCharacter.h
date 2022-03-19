#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiplayerFPSCharacter.generated.h"

UCLASS(config = Game)
class AMultiplayerFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMultiplayerFPSCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere)
	class UMultiplayerFPSHealthSystem* HealthSystem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* FullBodyMesh;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, Category = "Hitbox")
	class UBoxComponent* BodyHitboxBox;

	UPROPERTY(VisibleAnywhere, Category = "Hitbox")
	class UBoxComponent* HeadHitboxBox;

	UPROPERTY()
	TArray<class AMultiplayerFPSFirearm*> FirearmArray;

	UPROPERTY()
	TArray<bool> CanFireFirearmArray;

	UPROPERTY()
	bool bIsReloading;

	UPROPERTY()
	bool bIsZoomedIn;

	UPROPERTY()
	int16 WeaponInHand;

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AMultiplayerFPSFirearm>> FirearmClassArray;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float Value);

	virtual void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
		virtual void SprintStart();

	UFUNCTION(BlueprintCallable)
		virtual void SprintStop();

	UPROPERTY(Replicated)
		bool bIsSprinting;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		bool bDead;

	UPROPERTY()
		bool bIsInOptionsMenu;

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void SetOptionsMenuVisibility(bool Visibility);

	UFUNCTION()
		virtual void ToggleLeaderBoardVisibility();

	UFUNCTION()
		virtual void ToggleOptionsMenu();

	UPROPERTY()
		FString PlayerName;

	UFUNCTION()
		virtual void StartFiring();

	UFUNCTION()
		virtual void StopFiring();

	UFUNCTION()
		virtual void SwitchWeapon();

	UFUNCTION()
		virtual void SwitchFireMode();

	UFUNCTION()
		virtual void Reload();

	UFUNCTION()
		virtual void Zoom();

	UFUNCTION()
		virtual void ZoomOut();

	UFUNCTION()
		void SetFOV(float FOV);

	UFUNCTION()
		void HideFPMeshes();

	UFUNCTION()
		void ShowFPMeshes();

	UFUNCTION()
		void SetIsReloading();
};

