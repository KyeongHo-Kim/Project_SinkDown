#include "SinkDownProject/SubGame/MatrixWorldGame/HomingProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

AHomingProjectile::AHomingProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;
}

void AHomingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (ProjectileMesh)
	{
		//ProjectileMesh->SetSimulatePhysics(true);
		ProjectileMesh->SetNotifyRigidBodyCollision(true);
		ProjectileMesh->OnComponentHit.AddDynamic(this, &AHomingProjectile::OnHit);
	}

	// Find targets on startup
	GetWorld()->GetTimerManager().SetTimer(DelayTrackingHandle, this, &AHomingProjectile::FindTarget, HomingDelay, false);
}

void AHomingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActor)
	{
		HomingBehavior(DeltaTime);
	}
}

void AHomingProjectile::FindTarget()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		TargetActor = PC->GetPawn();
	}
}

void AHomingProjectile::HomingBehavior(float DeltaTime)
{
	// Calculate target direction
	FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	FVector CurrentDirection = GetActorForwardVector();

	// Interpolated new directions // // Apply rotation
	FVector NewDirection = FMath::VInterpNormalRotationTo(CurrentDirection, Direction, DeltaTime, TurnRate);
	SetActorRotation(NewDirection.Rotation());

	// Apply Location
	FVector NewLocation = GetActorLocation() + NewDirection * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}

void AHomingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
		if (HitCharacter)
		{
			FVector LaunchDirection = GetActorForwardVector() + FVector(0, 0, 0.5f); 
			LaunchDirection.Normalize();

			// LaunchCharacter(Velocity, XYOverride, ZOverride)
			HitCharacter->LaunchCharacter(LaunchDirection * 5000.0f, true, true);
		}
		Destroy();
	}
}

