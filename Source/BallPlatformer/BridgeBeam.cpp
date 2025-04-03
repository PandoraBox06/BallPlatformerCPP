// BridgeBeam.cpp
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
}

void ABridgeBeam::BeginPlay()
{
	Super::BeginPlay();
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
}

void ABridgeBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateForce();
}

void ABridgeBeam::UpdateForce()
{
	if (!StartConnector || !EndConnector) return;

	FVector Start = StartConnector->GetActorLocation();
	FVector End = EndConnector->GetActorLocation();

	FVector Delta = End - Start;
	float CurrentLength = Delta.Size();
	float Stretch = CurrentLength - RestLength;

	CurrentForce = FMath::Abs(SpringConstant * Stretch);

	bool bOverstressed = CurrentForce > MaxForce;
	SetVisualFeedback(bOverstressed);
}

void ABridgeBeam::SetVisualFeedback(bool bOverstressed)
{
	if (!Mesh) return;

	for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
	{
		UMaterialInstanceDynamic* DynMat = Mesh->CreateAndSetMaterialInstanceDynamic(i);
		if (DynMat)
		{
			DynMat->SetVectorParameterValue("Color", bOverstressed ? FLinearColor::Red : FLinearColor::White);
		}
	}
}
