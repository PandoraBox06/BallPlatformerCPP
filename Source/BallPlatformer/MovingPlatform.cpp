#include "MovingPlatform.h"

// Constructor
AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default values
	Speed = 200.0f;
	bLoop = true;
	CurrentWaypointIndex = 0;
}

// Called when the game starts
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Store the initial position
	StartPosition = GetActorLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move the platform each frame
	MovePlatform(DeltaTime);
}

// Moves the platform towards the next waypoint
void AMovingPlatform::MovePlatform(float DeltaTime)
{
	// Ensure there are waypoints to move between
	if (Waypoints.Num() == 0) return;

	// Calculate the target location relative to the start position
	FVector TargetLocation = StartPosition + Waypoints[CurrentWaypointIndex];

	// Get current location and movement direction
	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	// Check if the platform is close to the target waypoint
	if (FVector::Dist(CurrentLocation, TargetLocation) <= 10.0f) 
	{
		// Move to the next waypoint
		CurrentWaypointIndex++;

		// If we've reached the last waypoint, loop or stop
		if (CurrentWaypointIndex >= Waypoints.Num()) 
		{
			if (bLoop)
			{
				CurrentWaypointIndex = 0; // Restart from the first waypoint
			}
			else
			{
				return; // Stop movement
			}
		}
	}

	// Move the platform towards the target
	SetActorLocation(CurrentLocation + Direction * Speed * DeltaTime);
}
