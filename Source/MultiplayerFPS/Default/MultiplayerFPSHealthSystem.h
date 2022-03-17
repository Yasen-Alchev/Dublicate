#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MultiplayerFPSHealthSystem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERFPS_API UMultiplayerFPSHealthSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMultiplayerFPSHealthSystem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Shield")
	float MaxShield;

	UPROPERTY(EditAnywhere, Category = "Shield")
	float ShieldRechargeRate;

	UPROPERTY(EditAnywhere, Category = "Shield")
	float ShieldRechargeCooldownAfterDamage;

	UPROPERTY(EditAnywhere, Category = "Shield")
	float ShieldRechargeInterval;

	UPROPERTY(EditAnywhere, Category = "Shield")
	float ShieldRechargeRateIncreaseValue;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated)
	float CurrentHealth;

	UPROPERTY(Replicated)
	float CurrentShield;

	UPROPERTY()
	FTimerHandle ShieldRechargeStartTimer;

	UPROPERTY()
	FTimerHandle ShieldRechargeTimer;

	UPROPERTY()
	bool bShouldRechargeShield;

	UPROPERTY()
	int32 ShieldRateCounter;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable)
	void RechargeShield();

	UFUNCTION()
	void StartShieldRecharge();

	UFUNCTION(Server, Reliable)
	void TakeDamage(AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType, class AController* InstigatedBy,
		AActor* DamageCauser);

	FORCEINLINE float GetCurrentHealth()
	{
		return this->CurrentHealth;
	}

	FORCEINLINE float GetCurrentShield()
	{
		return this->CurrentShield;
	}

	FORCEINLINE float GetMaxHealth()
	{
		return this->MaxHealth;
	}

	FORCEINLINE float GetMaxShield()
	{
		return this->MaxShield;
	}

	UFUNCTION(Server, Reliable)
	void Heal(float Value);

	UFUNCTION(Server, Reliable)
	void Death();
};
