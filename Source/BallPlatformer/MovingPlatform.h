#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

/**
 * A platform that moves between specified waypoints.
 */
UCLASS()
class BALLPLATFORMER_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Constructor
	AMovingPlatform();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** List of waypoints the platform moves through */
	UPROPERTY(EditAnywhere, Category = "Movement")
	TArray<FVector> Waypoints;

	/** Movement speed of the platform */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed;

	/** Whether the platform loops back to the start after reaching the last waypoint */
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bLoop;

private:
	/** Index of the current waypoint */
	int CurrentWaypointIndex;

	/** Initial position of the platform */
	FVector StartPosition;
	
	/** Moves the platform along the waypoints */
	void MovePlatform(float DeltaTime);
};
