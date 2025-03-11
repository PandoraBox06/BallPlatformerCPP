#include "BallCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"

// Constructor
ABallCharacter::ABallCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and configure sphere component (main physics body)
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetEnableGravity(true);
	SphereComponent->SetLinearDamping(0.5f); // Prevents infinite rolling
	SphereComponent->SetAngularDamping(1.0f);

	// Create and attach the ball mesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(SphereComponent);

	// Initialize movement properties
	MoveForce = 500.0f;
	JumpImpulse = 500.0f;
	MaxJumps = 2;
	CurrentJumps = 0;
	DecelerationPower = 5.0f;
}

// Called when the game starts
void ABallCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Reset jumps at the start
	CurrentJumps = 0;
}

// Sets up player input bindings
void ABallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	// Get input subsystem and apply input mapping context
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMapping, 0);
	}

	// Bind input actions
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABallCharacter::Move);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABallCharacter::Jump);
		Input->BindAction(ToggleGravityAction, ETriggerEvent::Triggered, this, &ABallCharacter::ToggleGravity);
	}
}

// Handles movement input
void ABallCharacter::Move(const FInputActionValue& Value)
{
	// Get input vector
	FVector2D Axis2DValue = Value.Get<FVector2D>();
	MovementInput = FVector(Axis2DValue.X, Axis2DValue.Y, 0);

	// Normalize movement direction
	FVector MoveDirection = MovementInput.GetSafeNormal();

	// Apply movement force
	SphereComponent->AddForce(MoveDirection * MoveForce * SphereComponent->GetMass());
}

// Handles jump input
void ABallCharacter::Jump(const FInputActionValue& Value)
{
	if (CurrentJumps >= MaxJumps) return;

	// Apply an upward impulse for jumping
	FVector JumpVector = FVector(0, 0, JumpImpulse * SphereComponent->GetMass());
	SphereComponent->AddImpulse(JumpVector);

	// Increment jump count
	CurrentJumps++;
}

// Checks if the ball is on the ground
bool ABallCharacter::IsOnGround() const
{
	FHitResult Hit;
	FVector Start = SphereComponent->GetComponentLocation();
	FVector End = Start - FVector(0, 0, 50); // Short downward trace

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Perform line trace to check ground contact
	return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams);
}

// Called every frame
void ABallCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Reset jumps when the ball is on the ground
	if (IsOnGround()) 
	{
		CurrentJumps = 0;
	}

	// Apply deceleration when there is no input
	if (MovementInput.IsNearlyZero()) 
	{
		FVector CurrentVelocity = SphereComponent->GetPhysicsLinearVelocity();
		
		// Apply deceleration force only if moving
		if (!CurrentVelocity.IsNearlyZero(1.0f)) 
		{
			FVector DecelerationForce = -CurrentVelocity.GetSafeNormal() * (MoveForce * DecelerationPower);
			SphereComponent->AddForce(DecelerationForce);
		}
	}
}

// Toggles global gravity inversion
void ABallCharacter::ToggleGravity()
{
	// Get the world settings
	AWorldSettings* WorldSettings = GetWorld()->GetWorldSettings();
	if (!WorldSettings) return;

	// Ensure the world allows custom gravity changes
	WorldSettings->bGlobalGravitySet = true;

	// Invert gravity
	WorldSettings->GlobalGravityZ *= -1;
}
