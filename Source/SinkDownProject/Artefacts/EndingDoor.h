#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndingDoor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class ATriggerVolume;

UCLASS()
class SINKDOWNPROJECT_API AEndingDoor : public AActor
{
	GENERATED_BODY()
	
public:	

	AEndingDoor();

protected:
	virtual void BeginPlay() override;

private:
    bool bIsOpen = false;
 

protected:
    UPROPERTY(VisibleAnywhere) USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* DoorFrame;

    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Door;

    UPROPERTY(EditAnywhere, Category = "TriggerVolume") ATriggerVolume* TriggerVolume;

    UPROPERTY(EditAnywhere, Category = "Ending") bool bRequireAllDiaries = true;

    UFUNCTION()  void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
};
