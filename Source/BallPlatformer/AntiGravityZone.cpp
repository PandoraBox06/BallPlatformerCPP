#include "AntiGravityZone.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "BallCharacter.h"

// Constructor
AAntiGravityZone::AAntiGravityZone()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create the trigger box to detect when the ball enters the zone
	GravityTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("GravityTrigger"));
	RootComponent = GravityTrigger;
	GravityTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GravityTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	GravityTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	GravityTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Default gravity settings
	GravityDirection = FVector(0, 0, -1);  // Downward gravity
	GravityStrength = 980.0f;  // Earth's gravity (cm/s²)
}

// Called when the game starts
void AAntiGravityZone::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap events
	GravityTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAntiGravityZone::OnEnter);
	GravityTrigger->OnComponentEndOverlap.AddDynamic(this, &AAntiGravityZone::OnExit);
}

// Handles when a player enters the anti-gravity zone
void AAntiGravityZone::OnEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Cast the actor to ABallCharacter
	ABallCharacter* Ball = Cast<ABallCharacter>(OtherActor);
	if (Ball && Ball->SphereComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entered Anti-Gravity Zone!"));

		// Disable default gravity
		Ball->SphereComponent->SetEnableGravity(false);

		// Apply custom gravity force
		FVector CustomGravity = GravityDirection.GetSafeNormal() * GravityStrength;
		Ball->SphereComponent->AddForce(CustomGravity * Ball->SphereComponent->GetMass(), NAME_None, true);
	}
}

// Handles when a player exits the anti-gravity zone
void AAntiGravityZone::OnExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Cast the actor to ABallCharacter
	ABallCharacter* Ball = Cast<ABallCharacter>(OtherActor);
	if (Ball && Ball->SphereComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Exited Anti-Gravity Zone!"));

		// Re-enable default gravity
		Ball->SphereComponent->SetEnableGravity(true);

		// Reset damping to prevent unnatural movement
		Ball->SphereComponent->SetLinearDamping(0.5f);
	}
}
