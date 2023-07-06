// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomProjectile.h"
#include "ShooterGameState.h"
#include "ShooterPlayerState.h"
//#include "PlayerController.h"
#include "ShooterCPPGameMode.h"
#include "Public/HealthInterface.h"

// Sets default values
ACustomProjectile::ACustomProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereCollisionR = CreateDefaultSubobject <USphereComponent>("Sphere Component");
	SetRootComponent(SphereCollisionR);
	SphereCollisionR->InitSphereRadius(20);
	SphereCollisionR->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollisionR->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollisionR->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	SphereCollisionR->SetCollisionResponseToChannel(ECC_PhysicsBody,ECR_Block);
	SphereCollisionR->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	SphereCollisionR->SetNotifyRigidBodyCollision(true);
	SphereCollisionR->SetSimulatePhysics(true);
	Mesh = CreateDefaultSubobject <UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SphereCollisionR);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectComp = CreateDefaultSubobject <UProjectileMovementComponent>("Projectile Movement");
	ProjectComp->bShouldBounce = true;
	ProjectComp->InitialSpeed = 1500;
	ProjectComp->MaxSpeed = 6000;
	InitialLifeSpan=10;
}

// Called when the game starts or when spawned
void ACustomProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereCollisionR->OnComponentHit.AddDynamic(this, &ACustomProjectile::OnHit);
	
}

void ACustomProjectile::OnHit(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& HitResult)
{
	if (bDamageDoneToSomething)return;
	if (OtherActor) {
		FString TempString=OtherActor->GetName();
		//GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Emerald, FString::Printf(TEXT("Projectile hit %s"), OtherActor));
		IHealthInterface* TempInterface = Cast<IHealthInterface>(OtherActor);
		if (TempInterface) { 

			UE_LOG(LogTemp, Warning, TEXT(" - - - - - "));
			//Server_TestFunction(OtherActor, HitDamage);
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC) {
				AShooterPlayerState* PS = PC->GetPlayerState<AShooterPlayerState>();
				if (PS) {
					UE_LOG(LogTemp, Warning, TEXT("projectile pass to player state damage to target"));
					PS->Server_MakeDamageToActor(OtherActor, HitDamage);
					bDamageDoneToSomething = true;
					SphereCollisionR->OnComponentHit.Remove(this,"OnHit");
				}
			}
			return;
			if (HasAuthority() && false) { 
				TempInterface->MakeDamage(HitDamage);
				UE_LOG(LogTemp, Warning, TEXT("projectile has authority and made damage to interface"));
			}
			else {
				AShooterCPPGameMode* GM = Cast <AShooterCPPGameMode>(GetWorld()->GetAuthGameMode());
				//AShooterCPPGameMode* GM = GetWorld()->GetAuthGameMode<AShooterCPPGameMode>();
				if (GM) {
					UE_LOG(LogTemp, Warning, TEXT("Projectile calling game mode function to damage interface."));
					GM->PassDamageToState(OtherActor, HitDamage);
					
				}
				else	UE_LOG(LogTemp, Warning, TEXT("Projectile  not received game mode pointer. %d"), IsValid(GetWorld()->GetAuthGameMode()));
				
				/*
				AShooterGameState* TempState = Cast<AShooterGameState>(GetWorld()->GetGameState());
				if (TempState)TempState->Server_MakeDamageToInterface(OtherActor, HitDamage);
				UE_LOG(LogTemp,Warning,TEXT("projectile called game state function to damage interface"));*/
			}
		}
	}
}

// Called every frame
void ACustomProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomProjectile::Server_TestFunction_Implementation(AActor* Target, float Size)
{
	UE_LOG(LogTemp,Warning,TEXT("Server projectile test function implemented."));
	AShooterCPPGameMode* GM = Cast <AShooterCPPGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		UE_LOG(LogTemp, Warning, TEXT("Projectile calling game mode function to damage interface."));
		GM->PassDamageToState(Target, HitDamage);

	}
	else	UE_LOG(LogTemp, Warning, TEXT("Projectile  not received game mode pointer. %d"), IsValid(GetWorld()->GetAuthGameMode()));
	//IHealthInterface* HI = Cast<IHealthInterface>(Target);
	//if (HI)HI->MakeDamage(Size);
}

void ACustomProjectile::SetNetRole(ENetRole NewRole)
{
	SetRemoteRoleForBackwardsCompat(NewRole);
}