// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HealthInterface.h"
#include "ShooterGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERCPP_API AShooterGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION(Server,Reliable,WithValidation)void Server_MakeDamageToInterface(AActor* Target, float Size);
	void Server_MakeDamageToInterface_Implementation(AActor* Target, float Size);
	bool Server_MakeDamageToInterface_Validate(AActor* Target, float Size);
	/*UFUNCTION(NetMulticast, Reliable, WithValidation)void Multi_MakeDamageToInterface(IHealthInterface* Interface, float Size);
	void Multi_MakeDamageToInterface_Implementation(IHealthInterface* Interface, float Size);
	bool Multi_MakeDamageToInterface_Validate(IHealthInterface* Interface, float Size);
	*/
};
