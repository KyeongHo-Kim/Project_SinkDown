#include "SinkDownProject/SkillSystem/Melee/GlidingSkill.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGlidingSkill::UGlidingSkill()
{
    PrimaryComponentTick.bCanEverTick = true;

    bIsGliding = false;
    MinimumHeight = 200.0f;
    DescendingRate = 300.0f;
}

void UGlidingSkill::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsGliding)
    {
        DescentPlayer();
    }
}

void UGlidingSkill::ActivateSkill(ACharacter* Owner)
{
    Super::ActivateSkill(Owner);

    if (!Owner || !CharacterMovement) return;

    if (!bIsGliding)
    {
        StartGliding();
    }
    else
    {
        StopGliding();
    }
}

void UGlidingSkill::StartGliding()
{
    if (!CharacterMovement) return;

    if (CanStartGliding())
    {
        CurrentVelocity = CharacterMovement->Velocity;
        bIsGliding = true;

        PlaySkillEffects(GetOwner()->GetActorLocation());

        OnSkillStateChanged.Broadcast(bIsGliding);

        RecordOriginalSetting();

        CharacterMovement->RotationRate = FRotator(0.0f, DEFAULT_ROTATION_RATE_YAW, 0.0f);
        CharacterMovement->GravityScale = 0.0f;
        CharacterMovement->AirControl = DEFAULT_AIR_CONTROL;
        CharacterMovement->BrakingDecelerationFalling = DEFAULT_BRAKING_DECELERATION;
        CharacterMovement->MaxAcceleration = DEFAULT_MAX_ACCELERATION;
        CharacterMovement->MaxWalkSpeed = DEFAULT_MAX_WALK_SPEED;
        CharacterMovement->bUseControllerDesiredRotation = true;
    }
}

void UGlidingSkill::StopGliding()
{
    if (!CharacterMovement) return;

    ApplyOriginalSettings();
    bIsGliding = false;

    OnSkillStateChanged.Broadcast(bIsGliding);
}

bool UGlidingSkill::CanStartGliding()
{
    if (!CharacterMovement || !GetWorld()) return false;

    FHitResult Hit;
    FVector TraceStart = CharacterMovement->GetOwner()->GetActorLocation();
    FVector TraceEnd = TraceStart + CharacterMovement->GetOwner()->GetActorUpVector() * MinimumHeight * -1.0f;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(CharacterMovement->GetOwner());

    TEnumAsByte<ECollisionChannel> TracePerpertise = ECC_Visibility;
    GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TracePerpertise, QueryParams);

    return !Hit.bBlockingHit && CharacterMovement->IsFalling();
}

void UGlidingSkill::RecordOriginalSetting()
{
    if (!CharacterMovement) return;

    OriginalOrientRotation = CharacterMovement->bOrientRotationToMovement;
    OriginalGravityScale = CharacterMovement->GravityScale;
    OriginalAirControl = CharacterMovement->AirControl;
    OriginalDecelration = CharacterMovement->BrakingDecelerationFalling;
    OriginalAcceleration = CharacterMovement->MaxAcceleration;
    OriginalWalkingSpeed = CharacterMovement->MaxWalkSpeed;
    OriginalDesiredRotation = CharacterMovement->bUseControllerDesiredRotation;
}

void UGlidingSkill::DescentPlayer()
{
    if (!CharacterMovement) return;

    if (CharacterMovement->IsWalking())
    {
        StopGliding();
        return;
    }

    if (CurrentVelocity.Z != DescendingRate * -1.0f && bIsGliding)
    {
        float DeltaSeconds = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.0f;
        CurrentVelocity.Z = UKismetMathLibrary::FInterpTo(
            CurrentVelocity.Z,
            DescendingRate,
            DeltaSeconds,
            DEFAULT_INTERPOLATION_SPEED
        );
        CharacterMovement->Velocity.Z = DescendingRate * -1.0f;
    }
}

void UGlidingSkill::ApplyOriginalSettings()
{
    if (!CharacterMovement) return;

    CharacterMovement->bOrientRotationToMovement = OriginalOrientRotation;
    CharacterMovement->GravityScale = OriginalGravityScale;
    CharacterMovement->AirControl = OriginalAirControl;
    CharacterMovement->BrakingDecelerationFalling = OriginalDecelration;
    CharacterMovement->MaxAcceleration = OriginalAcceleration;
    CharacterMovement->MaxWalkSpeed = OriginalWalkingSpeed;
    CharacterMovement->bUseControllerDesiredRotation = OriginalDesiredRotation;
    CharacterMovement->RotationRate = FRotator(0.f, 500.f, 0.f);
}