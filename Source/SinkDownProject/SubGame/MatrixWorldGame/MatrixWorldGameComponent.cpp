#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldGameComponent.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemy.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemyWayPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/HealthSystem/HealthComponent.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldProjectile.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/HomingProjectile.h"
#include "SinkDownProject/Items/Rewards/Diary.h"
#include "SinkDownProject/Artefacts/PhysicsDoor.h"
#include "Engine/TriggerVolume.h"


UMatrixWorldGameComponent::UMatrixWorldGameComponent()
{
	GameDuration = 5.0f;
	InitialEnemySpeed = 600.0f;
	SpeedIncreaseRate = 50.0f;

	CurrentState = EMatrixWorldState::Inactive;
}


void UMatrixWorldGameComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (UHealthComponent* HealthComp = PlayerCharacter->FindComponentByClass<UHealthComponent>())
		{
			HealthComp->OnDeath.AddDynamic(this, &UMatrixWorldGameComponent::OnPlayerDeath);
		}
	}

	if (MatrixWorldDoor)
	{
		MatrixWorldDoor->SetActorHiddenInGame(true);
		MatrixWorldDoor->SetActorEnableCollision(false);
		MatrixWorldDoor->SetActorTickEnabled(false);

		MatrixWorldTriggerVolume->SetActorEnableCollision(false);
		MatrixWorldDoor->SetActorTickEnabled(false);
	}
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

void UMatrixWorldGameComponent::EndGame(bool Success)
{
	GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);


	if (Success)
	{
		CurrentState = EMatrixWorldState::Completed;

		if (ClearSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ClearSound);
		}

		CleanupGame();

		SpawnDiary(DiarySpawnOffset);

		if (MatrixWorldDoor)
		{
			MatrixWorldDoor->SetActorHiddenInGame(false);
			MatrixWorldDoor->SetActorEnableCollision(true);
			MatrixWorldDoor->SetActorTickEnabled(true);

			MatrixWorldTriggerVolume->SetActorEnableCollision(true);
			MatrixWorldDoor->SetActorTickEnabled(true);
		}
	}
	else
	{
		CurrentState = EMatrixWorldState::Failed;

		if (TargetEnemy)
		{
			TargetEnemy->Destroy();
			TargetEnemy = nullptr;
		}

		CurrentState = EMatrixWorldState::Inactive;

		RemainingTime = GameDuration;
		CurrentEnemySpeed = InitialEnemySpeed;
	}
}

void UMatrixWorldGameComponent::UpdateGame()
{
	RemainingTime -= 1.0f;
	UpdateEnemySpeed();

	if (RemainingTime <= 0)
	{
		EndGame(true);
	}
}

void UMatrixWorldGameComponent::OnPlayerDeath()
{
	if (CurrentState == EMatrixWorldState::Playing)
	{
		TArray<AActor*> MatrixProjectiles;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMatrixWorldProjectile::StaticClass(), MatrixProjectiles);
		for (AActor* Projectile : MatrixProjectiles)
		{
			Projectile->Destroy();
		}

		TArray<AActor*> HomingProjectiles;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHomingProjectile::StaticClass(), HomingProjectiles);
		for (AActor* Projectile : HomingProjectiles)
		{
			Projectile->Destroy();
		}

		EndGame(false);
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
}


ADiary* UMatrixWorldGameComponent::SpawnDiary(const FVector& Location)
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

