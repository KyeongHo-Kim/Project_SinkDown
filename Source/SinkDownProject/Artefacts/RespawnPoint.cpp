#include "SinkDownProject/Artefacts/RespawnPoint.h"

ARespawnPoint::ARespawnPoint()
{
	RespawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnLocation"));
	RootComponent = RespawnLocation;

	RespawnPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RespawnPointMesh"));
	RespawnPointMesh->SetupAttachment(RespawnLocation);
}


void ARespawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

