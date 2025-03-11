#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AntiGravityZone.generated.h"

/**
 * A zone that applies custom gravity effects to the player when entered.
 */
UCLASS()
class BALLPLATFORMER_API AAntiGravityZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Constructor
	AAntiGravityZone();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Collision box to detect when a character enters the zone */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* GravityTrigger;

	/** Direction in which gravity is applied (defaults to downward) */
	UPROPERTY(EditAnywhere, Category = "Gravity")
	FVector GravityDirection;

	/** Strength of the gravity applied in the zone */
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float GravityStrength;

	/** Called when an actor enters the anti-gravity zone */
	UFUNCTION()
	void OnEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when an actor exits the anti-gravity zone */
	UFUNCTION()
	void OnExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
