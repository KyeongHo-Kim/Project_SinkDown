#include "SinkDownProject/CombatSystem/Projectiles/MugunghwaProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"

AMugunghwaProjectile::AMugunghwaProjectile()
{
    if (ProjectileMovement)
    {
        ProjectileMovement->InitialSpeed = 6000.0f;
        ProjectileMovement->MaxSpeed = 6000.0f;
    }

    KnockbackForce = 3000.0f;
    UpwardForce = 500.0f;

    InitialLifeSpan = 2.0f;
}

void AMugunghwaProjectile::OnHit(AActor* HitActor, AActor* ProjectileOwner)
{
    Super::OnHit(HitActor, ProjectileOwner);
}

