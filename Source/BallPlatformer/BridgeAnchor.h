// BridgeAnchor.h
#pragma once

#include "CoreMinimal.h"
#include "BridgeElement.h"
#include "BridgeAnchor.generated.h"

UCLASS()
class BALLPLATFORMER_API ABridgeAnchor : public ABridgeElement
{
	GENERATED_BODY()

public:
	ABridgeAnchor();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
};
