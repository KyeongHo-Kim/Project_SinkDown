#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemy.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldEnemyWayPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldProjectile.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/HomingProjectile.h"

AMatrixWorldEnemy::AMatrixWorldEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMatrixWorldEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->bOrientRotationToMovement = true;
		MovementComp->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

		MovementComp->SetMovementMode(MOVE_Flying);
		MovementComp->GravityScale = 0.0f;
	}

	// Start firing projectiles
	GetWorld()->GetTimerManager().SetTimer(HomingProjectileTimerHandle, this, &AMatrixWorldEnemy::FireHomingProjectile, HomingProjectileInterval, true);
	GetWorld()->GetTimerManager().SetTimer(MatrixProjectileTimerHandle, this, &AMatrixWorldEnemy::FireMatrixProjectile, MatrixProjectileInterval, true);
}

void AMatrixWorldEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PathPoints.Num() == 0 || !PathPoints[CurrentPathIndex])
	{
		if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
		{
			MovementComp->Velocity = FVector::ZeroVector;
		}
		return;
	}

	FVector CurrentPoint = PathPoints[CurrentPathIndex]->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	// Check if the goal point has been reached
	if (FVector::Distance(CurrentLocation, CurrentPoint) < 200.0f)
	{
		OnReachedPathPoint();
		return;
	}

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		FVector Direction = (CurrentPoint - CurrentLocation).GetSafeNormal();
		MovementComp->Velocity = Direction * MovementComp->MaxWalkSpeed;

		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 5.0f));
	}
}

void AMatrixWorldEnemy::InitializePathPoints(const TArray<AMatrixWorldEnemyWayPoint*>& Points)
{
	PathPoints = Points;
	CurrentPathIndex = 0;
}


void AMatrixWorldEnemy::OnReachedPathPoint()
{
	MoveToNextPoint();
}

void AMatrixWorldEnemy::MoveToNextPoint()
{
	if (PathPoints.Num() > 0)
	{
		CurrentPathIndex = (CurrentPathIndex + 1) % PathPoints.Num();
	}
}

void AMatrixWorldEnemy::FireHomingProjectile()
{
	if (!GetWorld() || !HomingProjectileClass) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !PC->GetPawn()) return;

	FVector TargetLocation = PC->GetPawn()->GetActorLocation();
	FVector StartLocation = GetActorLocation() + FVector(200.0f, 0, 0);
	FVector Direction = (TargetLocation - StartLocation).GetSafeNormal();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	FRotator Rotation = Direction.Rotation();
	GetWorld()->SpawnActor<AHomingProjectile>(HomingProjectileClass,
		StartLocation, Rotation, SpawnParams);
}

void AMatrixWorldEnemy::FireMatrixProjectile()
{
	if (!GetWorld() || !MatrixWorldProjectileClass) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !PC->GetPawn()) return;

	FVector TargetLocation = PC->GetPawn()->GetActorLocation();
	FVector StartLocation = GetActorLocation() + FVector(200.0f, 0, 0);
	FVector Direction = (TargetLocation - StartLocation).GetSafeNormal();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ABaseProjectile* Projectile = GetWorld()->SpawnActor<AMatrixWorldProjectile>(
		MatrixWorldProjectileClass, StartLocation, Direction.Rotation(), SpawnParams);

	if (Projectile)
	{
		Projectile->Initialize(StartLocation, Direction);
	}
}