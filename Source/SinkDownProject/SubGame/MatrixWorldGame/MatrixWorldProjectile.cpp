#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"

AMatrixWorldProjectile::AMatrixWorldProjectile()
{
    PrimaryActorTick.bCanEverTick = true;


    // Set longer lifespan for slower projectile
    InitialLifeSpan = 10.0f;
}

void AMatrixWorldProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Modify inherited ProjectileMovement settings
    if (ProjectileMovement)
    {
        ProjectileMovement->InitialSpeed = ProjectileSpeed;
        ProjectileMovement->MaxSpeed = ProjectileSpeed;
        ProjectileMovement->bRotationFollowsVelocity = true;
        ProjectileMovement->bShouldBounce = false;
        ProjectileMovement->ProjectileGravityScale = 0.0f;
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingAccelerationMagnitude = 8000.0f;
    }

    TargetActor = FindTarget();

    if (ProjectileMovement && TargetActor)
    {
        ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
    }
}

void AMatrixWorldProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetActor)
    {
        UpdateProjectileDirection(DeltaTime);
    }
}

void AMatrixWorldProjectile::Initialize(const FVector& StartLocation, const FVector& Direction)
{
    Super::Initialize(StartLocation, Direction);

    // Additional initialization if needed
    TargetActor = FindTarget();
}

AActor* AMatrixWorldProjectile::FindTarget() const
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    return PC ? PC->GetPawn() : nullptr;
}

void AMatrixWorldProjectile::UpdateProjectileDirection(float DeltaTime)
{
    if (!ProjectileMovement || !TargetActor) return;

    // Calculate direction to target
    FVector DirectionToTarget = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

    // Interpolate current velocity towards target direction
    FVector NewVelocity = FMath::VInterpTo(ProjectileMovement->Velocity.GetSafeNormal(), DirectionToTarget, DeltaTime, TurnSpeed ) * ProjectileSpeed;

    ProjectileMovement->Velocity = NewVelocity;
    SetActorRotation(NewVelocity.Rotation());
}

void AMatrixWorldProjectile::OnHit(AActor* HitActor, AActor* ProjectileOwner)
{
    Super::OnHit(HitActor, ProjectileOwner);

    if (UHealthComponent* HealthComp = HitActor->FindComponentByClass<UHealthComponent>())
    {
        HealthComp->TakeMaxDamage();
    }
}
