// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/HealthInterface.h"
#include "Components/MaterialBillboardComponent.h"
//#include "Engine/EngineTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "EnemyVampire.generated.h"

UCLASS()
class SHOOTERCPP_API AEnemyVampire : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyVampire();
	UPROPERTY		(BlueprintReadWrite, EditDefaultsOnly) UBoxComponent* DamageCollision;
	UFUNCTION	(BlueprintCallable) void OnBoxOverlap (UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherComponentIndex, bool FromSwipe, const FHitResult& HitResult);
	class UAIPerceptionComponent* PerComp;
	UPROPERTY (BlueprintReadOnly, VisibleAnywhere)UAISenseConfig_Sight* SightConfig;
	UFUNCTION()void PerceptionUpdated(const TArray <AActor*>& Actors);
protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	bool ComingBack=true;
	FVector StartLocation;
	FVector Destination;
	bool PushSelf = false;
	void SetNewRotation(FVector LocationToLookAt);
	bool bDead = false;
	void DestroySelf();
	void CheckOverlapedTargets();
	FTimerHandle UnusedHandle=FTimerHandle();
	UPROPERTY(EditDefaultsOnly, Transient) UMaterialBillboardComponent* Billboard;
	UPROPERTY(EditDefaultsOnly) UMaterial* BillboardMaterial;
	UCurveFloat* EmptyCurve;
	UMaterialInstanceDynamic* HealthBarMaterialInstance;
	void GetLifetimeReplicatedProps(TArray <FLifetimeProperty>&OutLifetimeProps)const override;
	FAIStimulus LastStimulus;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MakeDamage(float Size) override;
	UPROPERTY(ReplicatedUsing = HealthChanged) float CurrentHealth = 100;
	UFUNCTION() void HealthChanged();
	AActor* CurrentTarget;
};
