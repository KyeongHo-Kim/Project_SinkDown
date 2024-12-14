#include "SinkDownProject/CombatSystem/Projectiles/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CollisionComponent->SetNotifyRigidBodyCollision(true);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bSimulationEnabled = true;

	InitialLifeSpan = 1.0f;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Binding the OnComponentHit function to a crash event
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnComponentHit);
	}
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseProjectile::Initialize(const FVector& StartLocation, const FVector& Direction)
{
	SetActorLocation(StartLocation);
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
}


void ABaseProjectile::OnHit(AActor* HitActor, AActor* ProjectileOwner)
{
	if (HitActor && HitActor != ProjectileOwner)
	{
		// Add Sound, Effect..
	}
	Destroy();
}

void ABaseProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* ProjectileOwner = GetOwner();

	if (OtherActor)
	{
		OnProjectileHit.Broadcast(OtherActor, ProjectileOwner, Hit.Location);
	}
	OnHit(OtherActor, ProjectileOwner);
}

