#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BridgeBeam.generated.h"

class ABridgeConnector;
class UPhysicsConstraintComponent;
class UMaterialInstanceDynamic;

UCLASS()
class BALLPLATFORMER_API ABridgeBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	ABridgeBeam();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* ConstraintStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* ConstraintEnd;

	// References to connectors
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Bridge")
	ABridgeConnector* StartConnector;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Bridge")
	ABridgeConnector* EndConnector;

	// Physics properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Physics")
	float RestLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Physics")
	float SpringConstant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Physics", meta = (ToolTip = "Maximum force the beam can withstand before breaking"))
	float MaxForce;
	
	// If true, MaxForce is interpreted as kg weight capacity and converted to Newtons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Physics", meta = (ToolTip = "If true, MaxForce is interpreted as kg weight capacity and converted to Newtons"))
	bool bMaxForceIsInKg = true;

	// Current state
	UPROPERTY(BlueprintReadOnly, Category = "Bridge|State")
	float CurrentForce;

	UPROPERTY(BlueprintReadOnly, Category = "Bridge|State")
	bool bBroken;

	// Debug options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Debug")
	bool bDebugForces;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge|Debug")
	float DebugDrawDuration;

	// Dynamic materials for visual feedback
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	// Functions
	void UpdateForce();
	void SetVisualFeedback(float StressLevel);
	void BreakBeam();

	// Blueprint event for when the beam breaks
	UFUNCTION(BlueprintNativeEvent, Category = "Bridge|Events")
	void OnBeamBroken();
	virtual void OnBeamBroken_Implementation();
};