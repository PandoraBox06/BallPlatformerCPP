// BridgeAnchor.cpp
#include "BridgeAnchor.h"

ABridgeAnchor::ABridgeAnchor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void ABridgeAnchor::BeginPlay()
{
	Super::BeginPlay();
}
