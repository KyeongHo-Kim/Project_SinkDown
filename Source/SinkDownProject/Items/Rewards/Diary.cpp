#include "SinkDownProject//Items/Rewards/Diary.h"
#include "Components/StaticMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ADiary::ADiary()
{
	PrimaryActorTick.bCanEverTick = true;

	DiaryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiaryMesh"));
	DiaryMesh->SetupAttachment(RootComponent);

    SpawnDuration = 1.0f;
    DestroyDuration = 1.0f;
    CurrentSpawnTime = 0.0f;
    CurrentDestroyTime = 0.0f;
    bIsSpawning = true;
    bIsDestroying = false;
}

void ADiary::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector::ZeroVector);
}

void ADiary::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsSpawning)
    {
        CurrentSpawnTime += DeltaTime;
        float InverseSpawnDuration = 1.0f / SpawnDuration; // Compute the inverse
        float Alpha = FMath::Clamp(CurrentSpawnTime * InverseSpawnDuration, 0.0f, 2.0f);

        // Apply the Ease Out Cubic Easing Function
        // t = 1 - (1-t)^3
        float EasedAlpha = 1.0f - FMath::Pow(1.0f - Alpha, 3.0f);

        SetActorScale3D(FVector(EasedAlpha));
        AddActorWorldRotation(FRotator(0.0f, 360.0f * DeltaTime, 0.0f));

        if (Alpha >= 1.0f)
        {
            bIsSpawning = false;
            SetActorScale3D(FVector::OneVector);
        }
    }
    else if (bIsDestroying)
    {
        CurrentDestroyTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentDestroyTime / DestroyDuration, 0.0f, 1.0f);

        // Ease In Cubic for destruction
        float EasedAlpha = FMath::Pow(1.0f - Alpha, 3.0f);

        SetActorScale3D(FVector(EasedAlpha));
        AddActorWorldRotation(FRotator(0.0f, 720.0f * DeltaTime, 0.0f)); 

        if (Alpha >= 1.0f)
        {
            Destroy();
        }
    }
}

void ADiary::StartDestruction()
{
    bIsDestroying = true;
    CurrentDestroyTime = 0.0f;

    // Play the effect
    if (InteractionEffect)
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            InteractionEffect,
            GetActorLocation(),
            GetActorRotation()
        );
    }
}

void ADiary::OnInteract()
{
    if (!bIsDestroying)
    {
        StartDestruction();
    }
}
