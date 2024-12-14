#include "SinkDownProject/SubGame/SteppingStonesGame/SteppingStoneGameComponent.h"
#include "SinkDownProject/SubGame/SteppingStonesGame/TrueGround.h"
#include "SinkDownProject/SubGame/SteppingStonesGame/FakeGround.h"

USteppingStoneGameComponent::USteppingStoneGameComponent()
{
}

// Stores the initial positions and rotations of all fake grounds when the game starts
void USteppingStoneGameComponent::BeginPlay()
{
    Super::BeginPlay();
    FakeGroundSpawnData.Empty();

    // Store initial positions of fake grounds
    for (AFakeGround* FakeGround : FakeGrounds)
    {
        if (FakeGround)
        {
            FFakeGroundData SpawnData;
            SpawnData.Location = FakeGround->GetActorLocation();
            SpawnData.Rotation = FakeGround->GetActorRotation();
            SpawnData.Class = FakeGround->GetClass();
            FakeGroundSpawnData.Add(SpawnData);
        }
    }
}

void USteppingStoneGameComponent::StartGame()
{
    RespawnFakeGrounds();
    ShowHints();
}

// Initiates the sequence of showing hints by clearing any existing timer and starting from the first true ground
void USteppingStoneGameComponent::ShowHints()
{
    GetWorld()->GetTimerManager().ClearTimer(SequentialGlowTimer);
    CurrentGlowIndex = 0;
    ShowNextGlow();
}

// Saves the current positions and rotations of all fake grounds
void USteppingStoneGameComponent::SaveFakeGroundLocations()
{
    FakeGroundSpawnData.Empty();

    for (AFakeGround* FakeGround : FakeGrounds)
    {
        if (FakeGround)
        {
            FFakeGroundData SpawnData;
            SpawnData.Location = FakeGround->GetActorLocation();
            SpawnData.Rotation = FakeGround->GetActorRotation();
            SpawnData.Class = FakeGround->GetClass();
            FakeGroundSpawnData.Add(SpawnData);
        }
    }
}

// Respawns fallen fake grounds at their original positions while avoiding overlaps with existing grounds
void USteppingStoneGameComponent::RespawnFakeGrounds()
{
    TArray<AFakeGround*> FakeGroundsToRemove;

    // Find and collect fallen fake grounds
    for (AFakeGround* FakeGround : FakeGrounds)
    {
        if (FakeGround && FakeGround->IsPhysicsEnabled())
        {
            FakeGroundsToRemove.Add(FakeGround);
        }
    }

    // Remove fallen grounds
    for (AFakeGround* FakeGroundToRemove : FakeGroundsToRemove)
    {
        int32 RemoveIndex = FakeGrounds.Find(FakeGroundToRemove);
        if (RemoveIndex != INDEX_NONE)
        {
            FakeGroundToRemove->Destroy();
            FakeGrounds.RemoveAt(RemoveIndex);
        }
    }

    // Spawn new fake grounds at original positions
    for (const FFakeGroundData& SpawnData : FakeGroundSpawnData)
    {
        bool ShouldSpawn = true;

        // Check if there's already a fake ground at this position
        for (AFakeGround* ExistingGround : FakeGrounds)
        {
            if (ExistingGround)
            {
                FVector Distance = ExistingGround->GetActorLocation() - SpawnData.Location;
                if (Distance.Size() < 50.0f)
                {
                    ShouldSpawn = false;
                    break;
                }
            }
        }

        if (ShouldSpawn)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = GetOwner();

            if (AFakeGround* NewFakeGround = GetWorld()->SpawnActor<AFakeGround>(
                SpawnData.Class,
                SpawnData.Location,
                SpawnData.Rotation,
                SpawnParams))
            {
                FakeGrounds.Add(NewFakeGround);
            }
        }
    }
}

// Shows the hint for the next true ground in sequence and sets up timer for the next one if available
void USteppingStoneGameComponent::ShowNextGlow()
{
    if (CurrentGlowIndex < TrueGrounds.Num() && TrueGrounds[CurrentGlowIndex])
    {
        TrueGrounds[CurrentGlowIndex]->ShowHint();
        CurrentGlowIndex++;

        if (CurrentGlowIndex < TrueGrounds.Num())
        {
            GetWorld()->GetTimerManager().SetTimer(
                SequentialGlowTimer,
                this,
                &USteppingStoneGameComponent::ShowNextGlow,
                DelayBetweenGlows,
                false
            );
        }
    }
}