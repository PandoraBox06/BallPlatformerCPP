#include "Bumper.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "BallCharacter.h"

// Constructor
ABumper::ABumper()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and set up the static mesh component (visual representation)
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	// Create and set up the collision box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Default bounce force
	BounceForce = 100.0f;
}

// Called when the game starts
void ABumper::BeginPlay()
{
	Super::BeginPlay();

	// Bind collision event
	CollisionBox->OnComponentHit.AddDynamic(this, &ABumper::OnHit);
}

// Handles collision with the ball character
void ABumper::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Attempt to cast the OtherActor to ABallCharacter
	ABallCharacter* Ball = Cast<ABallCharacter>(OtherActor);

	// If it's a valid ball and has a sphere component, apply impulse
	if (Ball && Ball->SphereComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bumper activated!")); // Debug log

		// Calculate bounce direction
		FVector BounceDirection = (Ball->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		// Apply impulse to the ball in the bounce direction
		Ball->SphereComponent->AddImpulse(BounceDirection * BounceForce, NAME_None, true);
	}
}
