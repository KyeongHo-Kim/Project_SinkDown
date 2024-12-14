#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnPoint.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class SINKDOWNPROJECT_API ARespawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ARespawnPoint();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere) USceneComponent* RespawnLocation;
	UPROPERTY(EditAnywhere, Category = "RespawnPointMesh") UStaticMeshComponent* RespawnPointMesh;
};
