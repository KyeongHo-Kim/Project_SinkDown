#include "SinkDownProject/SkillSystem/Melee/ParrySkill.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"

UParrySkill::UParrySkill()
{
	CooldownTime = 5.0f;
	ParryDuration = 0.4f;
	ReducedCooldownTime = 1.0f;
	ParryRadius = 100.0f;
	bIsParrying = false;
}

void UParrySkill::BeginPlay()
{
	Super::BeginPlay();

	SetupParryCollision();
}

void UParrySkill::ActivateSkill(ACharacter* Owner)
{
	if (!CanActivateSkill()) return;

	Super::ActivateSkill(Owner);
	StartParryWindow();
}

void UParrySkill::StartParryWindow()
{
	bIsParrying = true;
	OnSkillStateChanged.Broadcast(true);

	ParryCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ParryCollision->SetGenerateOverlapEvents(true);

	// Set timer to end parry window
	// Prevents timers from overlapping when using parry techniques in succession.
	if (ParryTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ParryTimerHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(ParryTimerHandle, this, &UParrySkill::OnParryEnd, ParryDuration, false);
}

void UParrySkill::OnParryEnd()
{
	bIsParrying = false;
	OnSkillStateChanged.Broadcast(false);

	ParryCollision->SetCollisionProfileName(TEXT("NoCollision"));
	ParryCollision->SetGenerateOverlapEvents(false);
}

void UParrySkill::SetupParryCollision()
{
	if (!OwnerCharacter) return;

	ParryCollision = NewObject<USphereComponent>(OwnerCharacter, TEXT("ParryCollision"));
	ParryCollision->SetSphereRadius(ParryRadius);
	ParryCollision->SetCollisionProfileName(TEXT("NoCollision")); 
	ParryCollision->SetGenerateOverlapEvents(false);  
	ParryCollision->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	ParryCollision->RegisterComponent();

	ParryCollision->OnComponentBeginOverlap.AddDynamic(this, &UParrySkill::HandleProjectileHit);
}

void UParrySkill::HandleProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsParrying || !OtherActor) return;

	UProjectileMovementComponent* ProjectileMovement = OtherActor->FindComponentByClass<UProjectileMovementComponent>();
	if (!ProjectileMovement) return;

	PlaySkillEffects(OtherActor->GetActorLocation());

	if (CooldownTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}

	// Reduced cooldown on successful parry
	CurrentCooldown = ReducedCooldownTime;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, [this]() { CurrentCooldown = 0.0f; }, ReducedCooldownTime, false);
	OtherActor->Destroy();
}

