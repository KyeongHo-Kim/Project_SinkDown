#include "SinkDownProject/Artefacts/SurveillanceLight.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SinkDownProject/CombatSystem/Projectiles/BaseProjectile.h"

ASurveillanceLight::ASurveillanceLight()
{
    PrimaryActorTick.bCanEverTick = true;

    SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
    RootComponent = SpotLight;

    SurveillanceLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SurveillanceLightMesh"));
    SurveillanceLightMesh->SetupAttachment(RootComponent);

    SpotLight->Intensity = 5000.0f;
    SpotLight->AttenuationRadius = 1000.0f;
    SpotLight->OuterConeAngle = 30.0f;
}

void ASurveillanceLight::BeginPlay()
{
    Super::BeginPlay();

    if (PatrolPoints.Num() > 0)
    {
        CurrentPatrolIndex = FMath::RandRange(0, PatrolPoints.Num() - 1);
        TargetRotation = PatrolPoints[CurrentPatrolIndex].Rotation;

        float RandomWaitTime = FMath::RandRange(1.0f, 3.0f);
        GetWorld()->GetTimerManager().SetTimer(
            PatrolTimerHandle,
            this,
            &ASurveillanceLight::MoveToNextPatrolPoint,
            RandomWaitTime,
            false
        );
    }

    if (SurveillanceLightMesh)
    {
        SurveillanceLightMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        SurveillanceLightMesh->SetSimulatePhysics(false); 
    }
}

void ASurveillanceLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateLightRotation(DeltaTime);
}

void ASurveillanceLight::MoveToNextPatrolPoint()
{
    if (PatrolPoints.Num() == 0) return;

    int32 NextIndex;
    do {
        NextIndex = FMath::RandRange(0, PatrolPoints.Num() - 1);
    } while (NextIndex == CurrentPatrolIndex && PatrolPoints.Num() > 1);

    CurrentPatrolIndex = NextIndex;
    TargetRotation = PatrolPoints[CurrentPatrolIndex].Rotation;

    float RandomWaitTime = FMath::RandRange(1.0f, 3.0f);
    GetWorld()->GetTimerManager().SetTimer(
        PatrolTimerHandle,
        this,
        &ASurveillanceLight::MoveToNextPatrolPoint,
        RandomWaitTime,
        false
    );
}

void ASurveillanceLight::UpdateLightRotation(float DeltaTime)
{
    FRotator CurrentRotation = SpotLight->GetComponentRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
    SpotLight->SetWorldRotation(NewRotation);
}

void ASurveillanceLight::EnablePhysics(AActor* ProjectileOwner, FVector ImpactPoint)
{

    GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);

    if (SurveillanceLightMesh)
    {
        // Change the root component (using DetachFromComponent)
        SurveillanceLightMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        SetRootComponent(SurveillanceLightMesh);
        SpotLight->AttachToComponent(SurveillanceLightMesh, FAttachmentTransformRules::KeepWorldTransform);

        // Enabling Physics
        SurveillanceLightMesh->SetSimulatePhysics(true);

        // Apply force in the projectile's collision direction
        float ImpulseStrength = 1000.0f; 
        FVector ImpulseDirection = (SurveillanceLightMesh->GetComponentLocation() - ImpactPoint).GetSafeNormal();
        SurveillanceLightMesh->AddImpulseAtLocation(ImpulseDirection * ImpulseStrength, ImpactPoint);
    }
}

void ASurveillanceLight::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    // Check for conflicts with project tiles
    if (ABaseProjectile* Projectile = Cast<ABaseProjectile>(Other))
    {
        EnablePhysics(Projectile->GetOwner(), HitLocation);
    }
}