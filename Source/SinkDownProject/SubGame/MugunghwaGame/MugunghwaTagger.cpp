#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaTagger.h"
#include "Components/StaticMeshComponent.h"

AMugunghwaTagger::AMugunghwaTagger()
{
	TaggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mugunghwa Tagger Mesh"));
	TaggerMesh->SetupAttachment(RootComponent);

	TurningSpeed = 360.0f;
	bIsTurning = false;

	ExplosionForce = 1000.0f;
	ExplosionRadius = 200.0f;
}

void AMugunghwaTagger::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetActorRotation();
	ResetRotation();
}

void AMugunghwaTagger::OnInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("void AMugunghwaTagger::OnInteract()"));
	OnMugunghwaInteract.Broadcast();
}

void AMugunghwaTagger::UpdateRotation()
{
	if (!bIsTurning) return;

	FRotator CurrentRotator = GetActorRotation();
	FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotator, TargetRotation, GetWorld()->GetDeltaSeconds(), TurningSpeed);

	SetActorRotation(NewRotation);

	// Check if the target rotation has been reached
	if (NewRotation.Equals(TargetRotation, 1.0f))
	{
		bIsTurning = false;
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
	}
}

void AMugunghwaTagger::StartCounting()
{
	// Rotate to initial position (wall view)
	ResetRotation();
}

void AMugunghwaTagger::StartTurning()
{
	TargetRotation = InitialRotation + FRotator(0.0f, 180.0f, 0.0f);
	bIsTurning = true;

	GetWorld()->GetTimerManager().SetTimer(
		RotationTimerHandle,
		this,
		&AMugunghwaTagger::UpdateRotation,
		0.016f,  // Approx. 60 fps
		true);
}

void AMugunghwaTagger::ResetRotation()
{
	SetActorRotation(InitialRotation);
	bIsTurning = false;
	GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
}

void AMugunghwaTagger::StartPhysicsDestruction()
{
	if (!TaggerMesh) return;


	TaggerMesh->SetSimulatePhysics(true);

	FVector RandomTorque = FVector(
		FMath::RandRange(-ExplosionForce, ExplosionForce),
		FMath::RandRange(-ExplosionForce, ExplosionForce),
		FMath::RandRange(-ExplosionForce, ExplosionForce)
	);
	TaggerMesh->AddTorqueInRadians(RandomTorque);

	FVector UpwardForce = FVector(0, 0, ExplosionForce * 2.0f);
	TaggerMesh->AddImpulse(UpwardForce);
}


