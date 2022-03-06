// Fill out your copyright notice in the Description page of Project Settings.


#include "CQ_Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACQ_Character::ACQ_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bIsInOptionsMenu = false;

	myMaterial = nullptr;
}

// Called when the game starts or when spawned
void ACQ_Character::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACQ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
