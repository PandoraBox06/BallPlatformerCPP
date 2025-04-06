// BridgeConnector.h
#pragma once

#include "CoreMinimal.h"
#include "BridgeElement.h"
#include "BridgeConnector.generated.h"

class ABridgeBeam;

UCLASS()
class BALLPLATFORMER_API ABridgeConnector : public ABridgeElement
{
	GENERATED_BODY()

public:
	ABridgeConnector();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;	

	UPROPERTY()
	TArray<ABridgeBeam*> ConnectedBeams;

	void AddBeam(ABridgeBeam* Beam);
    
	// Remove a beam from this connector
	void RemoveBeam(ABridgeBeam* Beam);
};
