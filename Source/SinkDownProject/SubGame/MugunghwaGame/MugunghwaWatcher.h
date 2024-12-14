#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MugunghwaWatcher.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class ATriggerVolume;
class AMugunghwaProjectile;

UCLASS()
class SINKDOWNPROJECT_API AMugunghwaWatcher : public AActor
{
	GENERATED_BODY()
	
public:	
	AMugunghwaWatcher();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere) USceneComponent* RootScene;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* WatcherMesh;
	UPROPERTY(EditAnywhere, Category = "Detection") ATriggerVolume* DetectionVolume;
	UPROPERTY(EditDefaultsOnly, Category = "Combat") TSubclassOf<AMugunghwaProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Combat") FVector ProjectileSpawnOffset;

public:
	UFUNCTION() void AttackPlayer();

};
