#include "SinkDownProject/SubGame/RollingBallGame/RollingBallGameComponent.h"
#include "SinkDownProject/SubGame/RollingBallGame/RollableBall.h"
#include "SinkDownProject/SubGame/RollingBallGame/RollingBallEndPoint.h"
#include "SinkDownProject/Items/Rewards/Diary.h"

URollingBallGameComponent::URollingBallGameComponent()
{
    CurrentState = ERollingBallState::Inactive;
}

void URollingBallGameComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URollingBallGameComponent::StartGame()
{
    if (!TargetEndPoint)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start game: EndPoint is missing"));
        return;
    }

    if (!BallClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ball class not set in RollingBallGameComponent"));
        return;
    }

    // Remove existing balls
    if (CurrentBall)
    {
        CurrentBall->OnReachedEndPoint.RemoveAll(this);
        CurrentBall->Destroy();
        CurrentBall = nullptr;
    }

    // Create a new ball
    FVector OwnerLocation = GetOwner()->GetActorLocation();
    FTransform BallTransform(OwnerLocation + BallSpawnOffset);
    CurrentBall = GetWorld()->SpawnActor<ARollableBall>(BallClass, BallTransform);

    if (CurrentBall)
    {
        UE_LOG(LogTemp, Warning, TEXT("CurrentBall"));
        CurrentBall->OnReachedEndPoint.AddDynamic(this, &URollingBallGameComponent::OnBallReachedEndPoint);
        CurrentBall->SetTargetEndPoint(TargetEndPoint);
    }

    CurrentState = ERollingBallState::Playing;
}

void URollingBallGameComponent::EndGame(bool bSuccess)
{
    if (CurrentBall)
    {
        CurrentBall->OnReachedEndPoint.RemoveAll(this);
        CurrentBall->Destroy();
        CurrentBall = nullptr;
    }

    if (TargetEndPoint)
    {
        FVector EndPointLocation = TargetEndPoint->GetActorLocation();

        if (bSuccess)
        {
            SpawnDiary(EndPointLocation);
        }

        TargetEndPoint->StartPhysicsDestruction();
        TargetEndPoint = nullptr;
    }

    CurrentState = bSuccess ? ERollingBallState::Completed : ERollingBallState::Failed;

    CurrentState = ERollingBallState::Inactive;
}

void URollingBallGameComponent::OnBallReachedEndPoint()
{
    EndGame(true);
}

ADiary* URollingBallGameComponent::SpawnDiary(const FVector& Location)
{
    if (!DiaryClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DiaryClass is not set in RollingBallGameComponent"));
        return nullptr;
    }

    // Spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Spawn the Diary
    ADiary* SpawnedDiary = GetWorld()->SpawnActor<ADiary>(DiaryClass, Location, FRotator::ZeroRotator, SpawnParams);

    if (!SpawnedDiary)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Diary"));
    }

    return SpawnedDiary;
}