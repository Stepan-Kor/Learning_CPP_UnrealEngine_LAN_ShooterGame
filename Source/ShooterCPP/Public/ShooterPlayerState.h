// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterGameInstance.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERCPP_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
		UFUNCTION(Server, Reliable) void Server_MakeDamageToActor(AActor* Target, float Size);
		void Server_MakeDamageToActor_Implementation(AActor* Target, float Size);
		UFUNCTION(NetMultiCast, Reliable)void Multi_PauseAllClients(bool NewPauseState);
		void Multi_PauseAllClients_Implementation(bool NewPauseState);
		UFUNCTION(Server, Reliable) void Server_SetPauseSession(bool NewState);
		void Server_SetPauseSession_Implementation(bool NewState);
		virtual void BeginPlay() override;
		void PauseSession(bool NewState);
protected:
	APlayerController* LocalPlayerController;
	UShooterGameInstance* GameInstance;
	
};
