#include "SinkDownProject/SubGame/MugunghwaGame/MugunghwaWatcher.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TriggerVolume.h"
#include "SinkDownProject/CombatSystem/Projectiles/MugunghwaProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AMugunghwaWatcher::AMugunghwaWatcher()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	// Create and attach mesh component
	WatcherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mugunghwa Tagger Mesh"));
	WatcherMesh->SetupAttachment(RootScene);

	DetectionVolume = nullptr;
	ProjectileSpawnOffset = FVector(400.0f, 0.0f, 3000.0f);
}

void AMugunghwaWatcher::BeginPlay()
{
	Super::BeginPlay();
}


void AMugunghwaWatcher::AttackPlayer()
{
	if (!ProjectileClass) return;

	// Get player locations
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter) return;

	// Projectile Spawn Location
	FVector SpawnLocation = GetActorLocation() + ProjectileSpawnOffset;
	FVector Direction = (PlayerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, 0.0f) - SpawnLocation).GetSafeNormal();

	// Create Projectile
	AMugunghwaProjectile* Projectile = GetWorld()->SpawnActor<AMugunghwaProjectile>(ProjectileClass, SpawnLocation, Direction.Rotation());

	if (Projectile)
	{
		Projectile->SetOwner(this);
		Projectile->Initialize(SpawnLocation, Direction);
	}
}
