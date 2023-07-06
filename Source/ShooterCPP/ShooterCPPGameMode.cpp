// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterCPPGameMode.h"
#include "ShooterCPPHUD.h"
#include "ShooterGameState.h"
#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

void AShooterCPPGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast <AShooterGameState>(GetWorld()->GetGameState());
}

AShooterCPPGameMode::AShooterCPPGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AShooterCPPHUD::StaticClass();
}

void AShooterCPPGameMode::PassDamageToState(AActor* Target, float Size)
{
	if (GameState) {
		UE_LOG(LogTemp, Warning, TEXT("Game mode passed damage func to game state"));
		GameState->Server_MakeDamageToInterface(Target, Size);
	}
	else UE_LOG(LogTemp, Warning, TEXT("Game mode have no pointer to game state"));
}