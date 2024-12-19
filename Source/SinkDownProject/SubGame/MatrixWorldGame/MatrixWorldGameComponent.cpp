#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldGameComponent.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemy.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemyWayPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


UMatrixWorldGameComponent::UMatrixWorldGameComponent()
{
	GameDuration = 30.0f;
	InitialEnemySpeed = 600.0f;
	SpeedIncreaseRate = 50.0f;

	CurrentState = EMatrixWorldState::Inactive;
}


void UMatrixWorldGameComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMatrixWorldGameComponent::StartGame()
{
	if (CurrentState != EMatrixWorldState::Inactive)
	{
		return;
	}

	RemainingTime = GameDuration;
	CurrentEnemySpeed = InitialEnemySpeed;
	CurrentState = EMatrixWorldState::Playing;

	SpawnEnemy();

	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &UMatrixWorldGameComponent::UpdateGame, 1.0f, true);
}

void UMatrixWorldGameComponent::EndGame()
{
	CurrentState = EMatrixWorldState::Completed;

	GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);

	CleanupGame();
}

void UMatrixWorldGameComponent::UpdateGame()
{
	RemainingTime -= 1.0f;
	UpdateEnemySpeed();

	if (RemainingTime <= 0)
	{
		EndGame();
	}
}

void UMatrixWorldGameComponent::SpawnEnemy()
{
	if (!EnemyClass || PathPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("The Enemy class is not set or the pathpoint is missing."));
		return;
	}

	FVector SpawnLocation = PathPoints[0]->GetActorLocation();
	FRotator SpawnRotation = PathPoints[0]->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	TargetEnemy = GetWorld()->SpawnActor<AMatrixWorldEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (TargetEnemy)
	{
		TargetEnemy->InitializePathPoints(PathPoints);
	}
}

void UMatrixWorldGameComponent::UpdateEnemySpeed()
{
	if (!TargetEnemy)
	{
		return;
	}

	CurrentEnemySpeed += SpeedIncreaseRate;

	if (UCharacterMovementComponent* MovementComponent = TargetEnemy->GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = CurrentEnemySpeed;
	}

}

void UMatrixWorldGameComponent::CleanupGame()
{
	// Remove Enemy
	if (TargetEnemy)
	{
		TargetEnemy->Destroy();
		TargetEnemy = nullptr;
	}

	// Remove path points
	for (AMatrixWorldEnemyWayPoint* Point : PathPoints)
	{
		if (Point)
		{
			Point->Destroy();
		}
	}
	PathPoints.Empty();

	CurrentState = EMatrixWorldState::Inactive;

	UE_LOG(LogTemp, Warning, TEXT("CleanupGame()"));
}


