// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameState.h"
#include "Net/UnrealNetwork.h"

void AShooterGameState::BeginPlay()
{
	Super::BeginPlay();	
}

void AShooterGameState::Server_MakeDamageToInterface_Implementation(AActor* Target, float Size)
{
	//GEngine->AddOnScreenDebugMessage(0456, 12, FColor::Magenta, "Shooter test interface function was called");
	UE_LOG(LogTemp, Warning, TEXT("Game state server damage func called"));
	if (IHealthInterface* HI = Cast <IHealthInterface>(Target))HI->MakeDamage(Size);
}

bool AShooterGameState::Server_MakeDamageToInterface_Validate(AActor* Target, float Size)
{
	return true;
}