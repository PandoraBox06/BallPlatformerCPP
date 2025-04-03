// BridgeConnector.h
#pragma once

#include "CoreMinimal.h"
#include "BridgeElement.h"
#include "BridgeConnector.generated.h"

// ✅ FORWARD DECLARE instead of including
class ABridgeBeam;

UCLASS()
class BALLPLATFORMER_API ABridgeConnector : public ABridgeElement
{
	GENERATED_BODY()

public:
	ABridgeConnector();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY()
	TArray<ABridgeBeam*> ConnectedBeams;

	void AddBeam(ABridgeBeam* Beam);
};
