#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bumper.generated.h"

/**
 * Bumper class that applies a bounce force to the player when hit.
 */
UCLASS()
class BALLPLATFORMER_API ABumper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Constructor
	ABumper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** The visual representation of the bumper */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* Mesh;

	/** The collision box that detects overlap events */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* CollisionBox;

	/** Force applied when the ball hits the bumper */
	UPROPERTY(EditAnywhere, Category = "Bumper")
	float BounceForce;

	/** Handles collision with the ball character */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
