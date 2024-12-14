#include "CombatComponent.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"
#include "SinkDownProject/CombatSystem/Projectiles/BaseProjectile.h"
#include "AIController.h"
#include "Components/SphereComponent.h"
#include "SinkDownProject/DataAsset/WeaponData.h"
#include "SinkDownProject/CombatSystem/TeamInterface.h"
#include "SinkDownProject/CombatSystem/CombatEffectManager.h"
#include "SinkDownProject/DataAsset/CombatEffectData.h"

UCombatComponent::UCombatComponent() 
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	//Creating an CombatEffects Manager
	CombatEffectManager = NewObject<UCombatEffectManager>(this);
	CombatEffectManager->RegisterComponent();

	if (EffectData)
	{
		CombatEffectManager->SetEffectData(EffectData);
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Functions to set the attack type
void UCombatComponent::SetAttackType(EAttackType NewAttackType)
{
	AttackType = NewAttackType;

	switch (AttackType)
	{
	case EAttackType::AT_Melee:
		AttackFunction.BindUObject(this, &UCombatComponent::MeleeAttack);
		break;
	case EAttackType::AT_Range:
		AttackFunction.BindUObject(this, &UCombatComponent::RangeAttack);
		break;
	default:
		AttackFunction.Unbind();
		break;
	}
}


void UCombatComponent::SetLocationDelegate(const FGetLocationDelegate& Delegate)
{
	GetLocationDelegate = Delegate;
}

void UCombatComponent::SetForwardVectorDelegate(const FGetForwardVectorDelegate& Delegate)
{
	GetForwardVectorDelegate = Delegate;
}

// Functions to determine whether a specific target can be attacked
bool UCombatComponent::CanAttack(AActor* TargetActor)
{
	if (!TargetActor) return false;

	UHealthComponent* HealthComp = Cast<UHealthComponent>(TargetActor->FindComponentByClass(UHealthComponent::StaticClass()));
	return HealthComp != nullptr && !HealthComp->IsDead();
}

void UCombatComponent::Attack(AActor* TargetActor)
{
	if (AttackFunction.IsBound())
	{
		AttackFunction.Execute(TargetActor);
	}
}

// Functions that perform melee attacks
void UCombatComponent::MeleeAttack(AActor* TargetActor)
{
	if (!GetLocationDelegate.IsBound() || !GetForwardVectorDelegate.IsBound()) return;

	FVector Start = GetLocationDelegate.Execute();
	FVector End = Start + GetOwner()->GetActorForwardVector() * WeaponRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	TArray<FHitResult> HitResults;
	if (GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(50.0f), QueryParams))
	{
		int32 AffectedTargets = 0;
		const int32 MaxTargets = 3;

		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor) continue;

			if (CanAttack(HitActor))
			{
				CombatEffectManager->PlayMeleeHitEffect(Hit.ImpactPoint, Hit.ImpactNormal);

				// Team check and damage application now handled in ApplyDamage
				ApplyDamage(HitActor, true);
				AffectedTargets++;

				if (AffectedTargets >= MaxTargets)
				{
					UE_LOG(LogTemp, Warning, TEXT("Maximum number of targets (%d) reached."), MaxTargets);
					break;
				}
			}
		}
	}
}

// Functions that perform ranged attacks
void UCombatComponent::RangeAttack(AActor* TargetActor)
{
	if (!GetLocationDelegate.IsBound() || !GetForwardVectorDelegate.IsBound()) return;

	if (CombatEffectManager) CombatEffectManager->PlayRangedFireSound();

	FVector SpawnLocation = GetLocationDelegate.Execute();
	FVector ShootDirection;

	if (TargetActor)
	{
		ShootDirection = (TargetActor->GetActorLocation() - SpawnLocation).GetSafeNormal();
	}
	else
	{
		ShootDirection = GetForwardVectorDelegate.Execute();
	}

	FActorSpawnParameters SpawnParams;
	AActor* OwnerActor = GetOwner();
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = Cast<APawn>(OwnerActor);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABaseProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(Projectiles, SpawnLocation, ShootDirection.Rotation(), SpawnParams);
	if (SpawnedProjectile)
	{
		SpawnedProjectile->OnProjectileHit.AddDynamic(this, &UCombatComponent::OnProjectileHit);

		UPrimitiveComponent* OwnerPrimitive = Cast<UPrimitiveComponent>(OwnerActor->GetRootComponent());
		if (OwnerPrimitive && SpawnedProjectile->CollisionComponent)
		{
			SpawnedProjectile->CollisionComponent->IgnoreActorWhenMoving(OwnerActor, true);
			OwnerPrimitive->IgnoreActorWhenMoving(SpawnedProjectile, true);
		}
	}
}

void UCombatComponent::ApplyDamage(AActor* TargetActor, bool IsMelee)
{
	if (!TargetActor) return;

	// Team check
	ITeamInterface* TargetTeam = Cast<ITeamInterface>(TargetActor);
	ITeamInterface* OwnerTeam = Cast<ITeamInterface>(GetOwner());

	// Only apply damage if actors are on different teams or if either actor doesn't implement team interface
	if (!TargetTeam || !OwnerTeam || TargetTeam->GetTeam() != OwnerTeam->GetTeam())
	{
		UHealthComponent* HealthComp = Cast<UHealthComponent>(TargetActor->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp)
		{
			HealthComp->TakeDamage(IsMelee);
		}
	}
}

void UCombatComponent::PlayAttackEffects()
{
	//..
}

// Determine if the crashed Actor is a player or destructible object and handle it
void UCombatComponent::OnProjectileHit(AActor* HitActor, AActor* ProjectileOwner, FVector HitLocation)
{
	if (HitActor && HitActor != ProjectileOwner)
	{
		if (CombatEffectManager) CombatEffectManager->PlayRangedHitSound(HitLocation);

		if (Cast<UHealthComponent>(HitActor->GetComponentByClass(UHealthComponent::StaticClass())))
		{
			// Team check and damage application handled in ApplyDamage
			ApplyDamage(HitActor, false);
		}
	}
}
