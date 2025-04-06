#include "BridgeBeam.h"
#include "BridgeConnector.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ABridgeBeam::ABridgeBeam()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);

	ConstraintStart = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintStart"));
	ConstraintStart->SetupAttachment(RootComponent);

	ConstraintEnd = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintEnd"));
	ConstraintEnd->SetupAttachment(RootComponent);
    
	// Default values for physics properties
	RestLength = 100.0f;
	SpringConstant = 1000.0f;
	MaxForce = 2000.0f; // Default in Newtons (200kg * ~9.8m/s²)
	bBroken = false;
	
	// Debug settings
	bDebugForces = true;
	DebugDrawDuration = 0.0f; // 0 means single frame
}

void ABridgeBeam::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize dynamic materials
	for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
	{
		UMaterialInstanceDynamic* DynMat = Mesh->CreateAndSetMaterialInstanceDynamic(i);
		DynamicMaterials.Add(DynMat);
	}
	
	// Calculate rest length if not set manually
	if (StartConnector && EndConnector && RestLength <= 0.0f)
	{
		FVector Start = StartConnector->GetActorLocation();
		FVector End = EndConnector->GetActorLocation();
		RestLength = (End - Start).Size();
	}
	
	if (StartConnector)
	{
		StartConnector->AddBeam(this);

		ConstraintStart->SetWorldLocation(StartConnector->GetActorLocation());
		ConstraintStart->SetConstrainedComponents(
			Cast<UPrimitiveComponent>(StartConnector->GetRootComponent()),
			NAME_None,
			Mesh,
			NAME_None
		);
	}

	if (EndConnector)
	{
		EndConnector->AddBeam(this);

		ConstraintEnd->SetWorldLocation(EndConnector->GetActorLocation());
		ConstraintEnd->SetConstrainedComponents(
			Cast<UPrimitiveComponent>(EndConnector->GetRootComponent()),
			NAME_None,
			Mesh,
			NAME_None
		);
	}
	
	// Convert MaxForce from kg to Newtons if using mass-based input
	// Assuming MaxForce is input as kg weight capacity
	if (bMaxForceIsInKg)
	{
		// Convert kg to Newtons (F = m*g)
		const float GravityAcceleration = 980.0f; // cm/s² in UE4 default scale
		MaxForce = MaxForce * GravityAcceleration;
		UE_LOG(LogTemp, Warning, TEXT("MaxForce converted from %f kg to %f Newtons"), MaxForce/GravityAcceleration, MaxForce);
	}
}

void ABridgeBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bBroken)
	{
		UpdateForce();
	}
}

void ABridgeBeam::UpdateForce()
{
	if (!StartConnector || !EndConnector) return;

	FVector Start = StartConnector->GetActorLocation();
	FVector End = EndConnector->GetActorLocation();

	FVector Delta = End - Start;
	float CurrentLength = Delta.Size();
	float Stretch = CurrentLength - RestLength;
	
	// Calculate tension force using Hooke's Law (F = k * x)
	float TensionForce = SpringConstant * Stretch;
	
	// Get the direction of the beam
	FVector Direction = Delta.GetSafeNormal();
	
	// Calculate the force vector
	FVector ForceVector = Direction * TensionForce;
	
	// Calculate the magnitude of the force (absolute value)
	CurrentForce = FMath::Abs(TensionForce);
	
	// Debug visualization
	if (bDebugForces)
	{
		// Draw a debug line showing the force
		FColor DebugColor = FColor::White;
		if (CurrentForce > MaxForce * 0.75f)
		{
			DebugColor = FColor::Red;
		}
		else if (CurrentForce > MaxForce * 0.5f)
		{
			DebugColor = FColor::Yellow;
		}
		else
		{
			DebugColor = FColor::Green;
		}
		
		// Draw a line representing the beam
		DrawDebugLine(GetWorld(), Start, End, DebugColor, false, DebugDrawDuration, 0, 2.0f);
		
		// Print the current force
		FString DebugText = FString::Printf(TEXT("Force: %.2f / %.2f N"), CurrentForce, MaxForce);
		DrawDebugString(GetWorld(), GetActorLocation(), DebugText, nullptr, DebugColor, DebugDrawDuration);
	}
	
	// Calculate stress level (0.0 to 1.0)
	float StressLevel = FMath::Clamp(CurrentForce / MaxForce, 0.0f, 1.0f);
	
	bool bOverstressed = CurrentForce > MaxForce;
	SetVisualFeedback(StressLevel);
	
	// Break the beam if it's overstressed
	if (bOverstressed && !bBroken)
	{
		UE_LOG(LogTemp, Warning, TEXT("Beam breaking! Force: %f, MaxForce: %f"), CurrentForce, MaxForce);
		BreakBeam();
	}
}

void ABridgeBeam::SetVisualFeedback(float StressLevel)
{
	if (DynamicMaterials.Num() == 0) return;

	// Create a gradient from white (0 stress) to yellow (medium stress) to red (max stress)
	FLinearColor StressColor;
	
	if (StressLevel < 0.5f)
	{
		// Interpolate from white to yellow
		StressColor = FLinearColor::LerpUsingHSV(FLinearColor::White, FLinearColor::Yellow, StressLevel * 2.0f);
	}
	else
	{
		// Interpolate from yellow to red
		StressColor = FLinearColor::LerpUsingHSV(FLinearColor::Yellow, FLinearColor::Red, (StressLevel - 0.5f) * 2.0f);
	}
	
	for (UMaterialInstanceDynamic* DynMat : DynamicMaterials)
	{
		if (DynMat)
		{
			DynMat->SetVectorParameterValue("Color", StressColor);
		}
	}
}

void ABridgeBeam::BreakBeam()
{
	bBroken = true;
	
	// Break the constraints
	if (ConstraintStart)
	{
		ConstraintStart->BreakConstraint();
	}
	
	if (ConstraintEnd)
	{
		ConstraintEnd->BreakConstraint();
	}
	
	// Notify the connectors
	if (StartConnector)
	{
		StartConnector->RemoveBeam(this);
	}
	
	if (EndConnector)
	{
		EndConnector->RemoveBeam(this);
	}
	
	// Set the material to a broken state
	for (UMaterialInstanceDynamic* DynMat : DynamicMaterials)
	{
		if (DynMat)
		{
			DynMat->SetVectorParameterValue("Color", FLinearColor(0.2f, 0.2f, 0.2f)); // Dark gray for broken beams
		}
	}
	
	// Optional: Play a breaking sound or particle effect
	OnBeamBroken();
}

void ABridgeBeam::OnBeamBroken_Implementation()
{
	// This can be overridden in Blueprints to add effects when the beam breaks
}