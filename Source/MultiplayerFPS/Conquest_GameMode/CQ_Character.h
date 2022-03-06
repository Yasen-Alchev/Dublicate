// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerFPS/Default/MultiplayerFPSTeamBasedCharacter.h"
#include "CQ_Character.generated.h"

UCLASS()
class MULTIPLAYERFPS_API ACQ_Character : public AMultiplayerFPSTeamBasedCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACQ_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};