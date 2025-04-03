// BridgeConnector.cpp
#include "BridgeConnector.h"

ABridgeConnector::ABridgeConnector()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void ABridgeConnector::AddBeam(ABridgeBeam* Beam)
{
	if (!ConnectedBeams.Contains(Beam))
	{
		ConnectedBeams.Add(Beam);
	}
}
