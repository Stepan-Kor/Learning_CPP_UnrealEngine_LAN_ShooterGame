// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "CustomProjectile.generated.h"

UCLASS()
class SHOOTERCPP_API ACustomProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomProjectile();
	// , Transient 
	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, Transient, Category = "Collision") USphereComponent* SphereCollisionR;
	UPROPERTY(EditDefaultsOnly) UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly) UProjectileMovementComponent* ProjectComp;
	float HitDamage = 35;
	bool bDamageDoneToSomething = true;
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable) void OnHit(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse, const FHitResult& HitResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Server, Reliable) void Server_TestFunction(AActor* Target, float Size);
	void Server_TestFunction_Implementation(AActor* Target, float Size);
	void SetNetRole(ENetRole NewRole);

};
