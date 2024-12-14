#include "PatrolPath.h"
#include "DrawDebugHelpers.h"

APatrolPath::APatrolPath() : bIsAssingned(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugPatrolPath();
}

void APatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void APatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (GetWorld())
	{
		DrawPatrolPath();
	}
}
#endif

void APatrolPath::DrawPatrolPath()
{
	if (Waypoints.Num() == 0) return;

	for (int32 i = 0; i < Waypoints.Num(); i++)
	{
		if (Waypoints[i] && IsValid(Waypoints[i]))
		{
			AActor* CurrentLocation = GetWaypoint(i);
			AActor* NextWaypoint = Waypoints[GetNextIndex(i)];

			if (NextWaypoint && IsValid(NextWaypoint))
			{
				FVector NextLocation = NextWaypoint->GetActorLocation();

				//Debug
				//DrawDebugSphere(GetWorld(), CurrentLocation->GetActorLocation(), 30.f, 12, FColor::Red, true);
				//DrawDebugLine(GetWorld(), CurrentLocation->GetActorLocation(), NextLocation, FColor::Green, true);
			}
		}
	}
}

int32 APatrolPath::GetNextIndex(int32 Index)
{
	return (Index + 1) % Waypoints.Num();
}

AActor* APatrolPath::GetWaypoint(int32 Index)
{
	if (Waypoints.IsValidIndex(Index))
	{
		return Waypoints[Index];
	}
	return nullptr;
}

void APatrolPath::DrawDebugPatrolPath()
{
	DrawPatrolPath();
}
