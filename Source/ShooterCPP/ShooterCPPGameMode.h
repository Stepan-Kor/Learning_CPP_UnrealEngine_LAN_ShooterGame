// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterCPPGameMode.generated.h"

UCLASS(minimalapi)
class AShooterCPPGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	AShooterCPPGameMode();
	void PassDamageToState(AActor* Target, float Size);
	class AShooterGameState* GameState;
};



