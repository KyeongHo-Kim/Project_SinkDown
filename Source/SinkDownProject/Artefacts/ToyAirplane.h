#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToyAirplane.generated.h"

class UStaticMeshComponent;
class ATriggerVolume;
class USoundBase; 

UCLASS()
class SINKDOWNPROJECT_API AToyAirplane : public AActor
{
	GENERATED_BODY()

public:
	AToyAirplane();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION() void OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	void EnablePhysics();

	UPROPERTY(EditAnywhere, Category = "Aeroplane") UStaticMeshComponent* AirplaneMesh;
	UPROPERTY(EditAnywhere, Category = "Trigger") ATriggerVolume* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* CrashSound;

	bool bIsPhysicsEnabled;
	bool bHasLanded;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};