// BridgeConnector.cpp
#include "BridgeConnector.h"

ABridgeConnector::ABridgeConnector()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}
void ABridgeConnector::AddBeam(ABridgeBeam* Beam)
{
	if (Beam && !ConnectedBeams.Contains(Beam))
	{
		ConnectedBeams.Add(Beam);
	}
}

void ABridgeConnector::RemoveBeam(ABridgeBeam* Beam)
{
	if (Beam)
	{
		// Remove the beam from the connected beams array
		ConnectedBeams.Remove(Beam);
        
		// Optional: Log the removal for debugging
		UE_LOG(LogTemp, Verbose, TEXT("Beam removed from connector %s. Remaining beams: %d"), 
			*GetName(), ConnectedBeams.Num());
		
	}
}