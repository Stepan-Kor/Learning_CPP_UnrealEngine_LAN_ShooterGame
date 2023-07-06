// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyVampire.h"
#include "PlayerCharacter.h"
#include "Math/Vector.h"
#include "Net/UnrealNetwork.h"

#include "Components/CapsuleComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyVampire::AEnemyVampire()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	GetMesh()->SetRelativeLocation(FVector(0,0,-88));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	PrimaryActorTick.bCanEverTick = true;
	DamageCollision = CreateDefaultSubobject <UBoxComponent>(TEXT("Damage Box Collision"));
	DamageCollision->SetupAttachment(RootComponent);
	PerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	Billboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Health Billboard"));
	Billboard->SetupAttachment(RootComponent);
	Billboard->SetRelativeLocation(FVector(0,0,120));

	SightConfig = CreateDefaultSubobject <UAISenseConfig_Sight>(TEXT("Sight Configureation"));
	SightConfig->SightRadius = 1500;
	SightConfig->LoseSightRadius = 1500;
	SightConfig->PeripheralVisionAngleDegrees = 90;
	SightConfig->SetMaxAge (0.2);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerComp->ConfigureSense(*SightConfig);
	PerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	PerComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyVampire::PerceptionUpdated);
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyVampire::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AMultiPlayerCPPCharacter, ShotsFired);
	DOREPLIFETIME(AEnemyVampire, CurrentHealth);
}

// Called when the game starts or when spawned
void AEnemyVampire::BeginPlay()
{ 
	Super::BeginPlay();
	AController* Contr = GetController();
	if (Contr) { UE_LOG(LogTemp, Warning, TEXT("Vampire have controller %s"), *Contr->GetName()); }
	else { UE_LOG(LogTemp, Warning, TEXT("Vampire have no controller")); }

	if (BillboardMaterial && !IsValid(HealthBarMaterialInstance)) {
		HealthBarMaterialInstance = UMaterialInstanceDynamic::Create(BillboardMaterial, this);
		Billboard->Elements.Empty();
		Billboard->AddElement(HealthBarMaterialInstance, EmptyCurve, false, 3, 60, EmptyCurve);
	}
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyVampire::OnBoxOverlap);
	/*
	if (IsValid(PerceptionComp))PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyVampire::PerceptionUpdated);
	else { UE_LOG(LogTemp, Warning, TEXT("Enemy have not pointer to preception component")); 
	PerceptionComp = Cast<UAIPerceptionComponent>(GetComponentByClass(UAIPerceptionComponent::StaticClass()));
	if (!IsValid(PerceptionComp)) { UE_LOG(LogTemp, Warning, TEXT("Enemy STILL have not pointer to preception component")); }
	else UE_LOG(LogTemp, Warning, TEXT("Enemy GOT pointer to preception component"));
	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyVampire::PerceptionUpdated);
	}*/
	StartLocation = GetActorLocation();
	//UE_LOG(LogTemp,Warning, IsValid(PerceptionComp)? TEXT("true") : TEXT("false"));
	//GEngine->AddOnScreenDebugMessage(1,20,FColor::Red, IsValid(PerComp) ? "Second pointer valid":"Second pointer not valid");
}

void AEnemyVampire::SetNewRotation(FVector LocationToLookAt)
{
	FVector TempVector = LocationToLookAt - GetActorLocation();
	TempVector.Z = 0;
	SetActorRotation(TempVector.Rotation());
}



// Called every frame
void AEnemyVampire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PushSelf && !bDead) {
		FVector TempVector;
		if (ComingBack) {
			if (FVector::Distance(StartLocation, GetActorLocation()) > 75) {
				TempVector = (StartLocation - GetActorLocation()).GetClampedToSize(922220.f, 922220.f);
				TempVector.Z = 0;
				GetCharacterMovement()->AddForce(TempVector);
			}
			else { PushSelf = false; ComingBack = false; }
		}
		else {
			if (FVector::Distance(Destination, GetActorLocation()) > 135) {
				TempVector = (Destination - GetActorLocation()).GetClampedToSize(922220.f, 922220.f);
				TempVector.Z = 0;
				GetCharacterMovement()->AddForce(TempVector);
			}
			else 	PushSelf = false;
		}
	}
}

// Called to bind functionality to input
void AEnemyVampire::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyVampire::MakeDamage(float Size)
{
	//SomeInteger++;
	//UE_LOG(LogTemp,Warning,TEXT("vampires health interface integer %i"),SomeInteger);
	if (bDead) return;
	CurrentHealth -= Size;
	if (HasAuthority())HealthChanged();
	else { 
		
		UE_LOG(LogTemp, Warning, TEXT("vampire from client got damaged %f"), CurrentHealth); 
	}
	


}

void AEnemyVampire::HealthChanged()
{
	//GEngine->AddOnScreenDebugMessage(0,2,FColor::Orange,FString::Printf(TEXT("Vampire got damage %f"),Size));
	if (HealthBarMaterialInstance)HealthBarMaterialInstance->SetScalarParameterValue("Percent", CurrentHealth / 100);
	else if (BillboardMaterial ) {
		HealthBarMaterialInstance = UMaterialInstanceDynamic::Create(BillboardMaterial, this);
		Billboard->Elements.Empty();
		Billboard->AddElement(HealthBarMaterialInstance, EmptyCurve, false, 3, 60, EmptyCurve);
		HealthBarMaterialInstance->SetScalarParameterValue("Percent", CurrentHealth / 100);
	}
	if (CurrentHealth < 0) {
		bDead = true;
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Billboard->DestroyComponent();
		UE_LOG(LogTemp, Warning, TEXT("Vampire trigered destruction timer"));
		FTimerHandle TempHandle;
		GetWorldTimerManager().SetTimer(TempHandle, this, &AEnemyVampire::DestroySelf, 5.0f, false);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Vampire updated health %s."),*this->GetName());
		if (HasAuthority()) {
			UE_LOG(LogTemp, Warning, TEXT("Vampire %s has authority."), *this->GetName());
			FActorPerceptionBlueprintInfo TempPInfo;
			if (PerComp->GetActorsPerception(CurrentTarget, TempPInfo)) {
				if (!TempPInfo.LastSensedStimuli[0].IsActive()) {
					SetActorRotation(FRotator(0,FMath::FRandRange(0,360),0));
					UE_LOG(LogTemp, Warning, TEXT("Vampire got random rotation."));
				}
				else UE_LOG(LogTemp, Warning, TEXT("Vampire chasing something."));
			}
			else { 
				UE_LOG(LogTemp, Warning, TEXT("Vampire have nothing on current target."));
				SetActorRotation(FRotator(0, FMath::FRandRange(0, 360), 0));			
			}

		}
	}
	
}


void AEnemyVampire::OnBoxOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherComponentIndex, bool FromSwipe, const FHitResult& HitResult)
{
	//UE_LOG(LogTemp,Warning,TEXT("vampire overlaped some thing"));
	if (!IsValid(OtherActor))return;
	APlayerCharacter* Player = Cast <APlayerCharacter>(OtherActor);
	if (IsValid(Player) ){
		Player->MakeDamage(35);
		/*
		FString TempString = "Vampire ";
		TempString.Append(this->GetName());
		TempString.Append(" overlaped object ");
		TempString.Append(OtherActor->GetName());
		TempString.Append(" with distance ");
		TempString.Append(FString::SanitizeFloat((OtherActor->GetActorLocation()-this->GetActorLocation()).Size()));
		//GEngine->AddOnScreenDebugMessage(0,225,FColor::Magenta,TempString);
		UE_LOG(LogTemp, Warning, TEXT("%s"),*TempString);
		*/
		
		if (!Player->bDead) {
			GetWorldTimerManager().SetTimer(UnusedHandle,this,&AEnemyVampire::CheckOverlapedTargets,0.5f,false);
			}
		}
}

void AEnemyVampire::PerceptionUpdated(const TArray<AActor*>& Actors)
{
	if (!HasAuthority() || bDead) return;
	FActorPerceptionBlueprintInfo TempPInfo;
	if (PerComp->GetActorsPerception(CurrentTarget, TempPInfo)) {
		if (TempPInfo.LastSensedStimuli[0].IsActive()) {
			Destination = CurrentTarget->GetActorLocation();
			PushSelf = true;
			ComingBack = false;
			SetNewRotation(Destination);
			//UE_LOG(LogTemp, Warning, TEXT("Vampire target on list."));
			return;
		}
	}

	int32 TempInt = 0;
	bool foundsomeone=false;
	//UE_LOG(LogTemp, Warning, TEXT("Vampire target is NOT on list."));
		for (AActor* Suspect : Actors) {
			if (IsValid(Suspect)) {
				TempInt++;
				foundsomeone = true;
				APlayerCharacter* PlChar = Cast<APlayerCharacter>(Suspect);
				if (IsValid(PlChar)) {
					FActorPerceptionBlueprintInfo TempInfo;
					if (PerComp) PerComp->GetActorsPerception(PlChar, TempInfo);
					else { 
						//UE_LOG(LogTemp, Warning, TEXT("Vampire have empty pointer to perception component"));
						break; }

					
					if (TempInfo.LastSensedStimuli[0].WasSuccessfullySensed()) {
						//UE_LOG(LogTemp, Warning, TEXT("Player got in sight"));
						//GetCharacterMovement()->Velocity = FVector(120,0,0);
						LastStimulus = TempInfo.LastSensedStimuli[0];
						Destination = PlChar->GetActorLocation();
						CurrentTarget = PlChar;
						PushSelf = true;
						ComingBack = false;
						SetNewRotation(Destination);
						break;
					}

				}
				//else UE_LOG(LogTemp, Warning, TEXT("no player detected"));
			}
			//else UE_LOG(LogTemp, Warning, TEXT("no actor detected"));
		}
		//UE_LOG(LogTemp, Warning, TEXT(" got some thing .  .  .  %i"), TempInt);
		if (!foundsomeone) {
			ComingBack = true;
			PushSelf = true;
			SetNewRotation(StartLocation);
		}
	

}

void AEnemyVampire::DestroySelf()
{
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Vampire destroing self"));
}

void AEnemyVampire::CheckOverlapedTargets()
{
	TArray <AActor*> Actors;
	DamageCollision->GetOverlappingActors(Actors, APlayerCharacter::StaticClass());
	APlayerCharacter* Player;
	bool DamageDone = false;
	for (AActor* Actor : Actors) {
		Player = Cast <APlayerCharacter>(Actor);
		if (IsValid(Player)) { Player->MakeDamage(35); DamageDone = true; }
	}
	if (DamageDone) {
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AEnemyVampire::CheckOverlapedTargets, 0.5f, false);
	}
}
