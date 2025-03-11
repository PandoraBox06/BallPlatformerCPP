#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "BallCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * A player-controlled ball character that moves and jumps using physics.
 */
UCLASS()
class BALLPLATFORMER_API ABallCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Constructor
	ABallCharacter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets up player input bindings
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Sphere component for physics-based movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	/** Static mesh representing the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BallMesh;

	/** Input mapping context for enhanced input system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;
	
	/** Input action for movement */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	
	/** Input action for jumping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	
	/** Input action for toggling gravity */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ToggleGravityAction;
	
	/** Force applied when jumping */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpImpulse;

	/** Force applied for movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveForce;
	
	/** Maximum number of jumps allowed before landing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int MaxJumps;
	
	/** Deceleration force applied when no input is detected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DecelerationPower;

	/** Toggles global gravity inversion */
	UFUNCTION()
	void ToggleGravity();

private:
	/** Stores movement input */
	FVector MovementInput;

	/** Current jump count */
	int CurrentJumps;

	/** Checks if the ball is on the ground */
	bool IsOnGround() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Handles movement input */
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/** Handles jump input */
	UFUNCTION()
	void Jump(const FInputActionValue& Value);
};
