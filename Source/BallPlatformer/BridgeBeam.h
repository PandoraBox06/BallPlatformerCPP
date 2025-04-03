// BridgeBeam.h
#pragma once

#include "CoreMinimal.h"
#include "BridgeElement.h"
#include "BridgeBeam.generated.h"

// ✅ FORWARD DECLARE
class ABridgeConnector;

UCLASS()
class BALLPLATFORMER_API ABridgeBeam : public ABridgeElement
{
	GENERATED_BODY()

public:
	ABridgeBeam();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category="Bridge")
	ABridgeConnector* StartConnector;

	UPROPERTY(EditAnywhere, Category="Bridge")
	ABridgeConnector* EndConnector;

	UPROPERTY(EditAnywhere, Category="Bridge|Physics")
	float MaxForce = 1000.f;

	UPROPERTY(EditAnywhere, Category="Bridge|Physics")
	float SpringConstant = 10.f;

	UPROPERTY(EditAnywhere, Category="Bridge|Physics")
	float RestLength = 100.f;

	float CurrentForce;

	void UpdateForce();
	void SetVisualFeedback(bool bOverstressed);

	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* ConstraintStart;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* ConstraintEnd;

};
