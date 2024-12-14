#include "SinkDownProject/Artefacts/ToyAirplane.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/CombatSystem/Projectiles/BaseProjectile.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

AToyAirplane::AToyAirplane()
{
	PrimaryActorTick.bCanEverTick = true;

	AirplaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AirplaneMesh"));
	RootComponent = AirplaneMesh;

	AirplaneMesh->SetSimulatePhysics(false);
	AirplaneMesh->SetEnableGravity(false);
}

void AToyAirplane::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerVolume)
	{
		TriggerVolume->OnActorBeginOverlap.AddDynamic(this, &AToyAirplane::OnActorBeginOverlap);
	}

	if (AirplaneMesh)
	{
		AirplaneMesh->OnComponentHit.AddDynamic(this, &AToyAirplane::OnHit);
	}
}

void AToyAirplane::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Cast<ABaseProjectile>(Other))
	{
		EnablePhysics();
	}
}

void AToyAirplane::OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ACharacter>(OtherActor))
	{
		EnablePhysics();
	}
}

void AToyAirplane::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsPhysicsEnabled)
	{
		if (!bHasLanded && CrashSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				CrashSound,
				GetActorLocation(),
				1.0f,
				1.0f
			);
		}

		if (ASinkDownProjectCharacter* Player = Cast<ASinkDownProjectCharacter>(OtherActor))
		{
			if (!bHasLanded)
			{
				if (UHealthComponent* HealthComp = Player->FindComponentByClass<UHealthComponent>())
				{
					HealthComp->TakeMaxDamage();
				}
				// Setting bHasLanded to true after colliding with a player
				bHasLanded = true;
			}
		}
		else if (OtherComp->IsSimulatingPhysics() == false)
		{
			bHasLanded = true;
		}
	}
}

void AToyAirplane::EnablePhysics()
{
	if (!bIsPhysicsEnabled)
	{
		AirplaneMesh->SetSimulatePhysics(true);
		AirplaneMesh->SetEnableGravity(true);
		AirplaneMesh->SetMassScale(NAME_None, 100.0f);
		bIsPhysicsEnabled = true;
	}
}