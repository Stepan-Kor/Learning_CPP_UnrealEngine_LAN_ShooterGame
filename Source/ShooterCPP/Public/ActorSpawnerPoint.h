// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorSpawnerPoint.generated.h"

UCLASS()
class SHOOTERCPP_API AActorSpawnerPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorSpawnerPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditInstanceOnly) TSubclassOf<AActor> ClassOfActorToSpawn;
	UPROPERTY (EditInstanceOnly) FVector AdditionalVector=FVector (0,0,0);
	UFUNCTION() void ActorDestroyed(AActor* DestroyedActor);

};
