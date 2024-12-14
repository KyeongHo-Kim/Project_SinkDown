#include "SinkDownProject/SubGame/SteppingStonesGame/TrueGround.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownProject/Player/SinkDownProjectCharacter.h"

ATrueGround::ATrueGround()
{
    PrimaryActorTick.bCanEverTick = true;

    GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground Mesh"));
    RootComponent = GroundMesh;

    GlowDuration = 1.0f;
    MaxGlowIntensity = 5.0f;
    GlowColor = FLinearColor(0.5f, 1.0f, 1.0f, 1.0f);
    bIsGlowing = false;
    GlowTime = 0.0f;
    bCanPlaySound = true;
}

// Set up the dynamic material instance and initialize glow parameters
void ATrueGround::BeginPlay()
{
    Super::BeginPlay();

    if (GroundMesh && GroundMaterial)
    {
        DynamicMaterial = UMaterialInstanceDynamic::Create(GroundMaterial, this);
        GroundMesh->SetMaterial(0, DynamicMaterial);

        if (DynamicMaterial)
        {
            DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveIntensity"), 0.0f);
            DynamicMaterial->SetVectorParameterValue(TEXT("EmissiveColor"), GlowColor);
        }
    }

    OnActorHit.AddDynamic(this, &ATrueGround::OnHit);
}

void ATrueGround::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    if (ASinkDownProjectCharacter* Character = Cast<ASinkDownProjectCharacter>(OtherActor))
    {
        if (bCanPlaySound && StepSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                StepSound,
                GetActorLocation(),
                1.0f,
                1.0f
            );

            bCanPlaySound = false;
            GetWorld()->GetTimerManager().SetTimer(
                SoundCooldownTimer,
                [this]() { bCanPlaySound = true; },
                0.2f,
                false
            );
        }
    }
}

// Update the glow effect animation each frame
void ATrueGround::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsGlowing && DynamicMaterial)
    {
        GlowTime += DeltaTime;

        float NormalizedTime = GlowTime / GlowDuration;
        float GlowIntensity = MaxGlowIntensity * FMath::Sin(NormalizedTime * PI);

        if (GlowIntensity <= 0.0f)
        {
            EndHintEffect();
        }
        else
        {
            DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveIntensity"), GlowIntensity);
        }
    }
}

// Start the hint glow effect
void ATrueGround::ShowHint()
{
    if (DynamicMaterial)
    {
        bIsGlowing = true;
        GlowTime = 0.0f;
    }
}

// End the hint glow effect
void ATrueGround::EndHintEffect()
{
    bIsGlowing = false;
    if (DynamicMaterial)
    {
        DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveIntensity"), 0.0f);
    }
}