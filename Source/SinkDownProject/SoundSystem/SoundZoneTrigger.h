// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SoundZoneTrigger.generated.h"


UCLASS()
class SINKDOWNPROJECT_API ASoundZoneTrigger : public ATriggerBox
{
    GENERATED_BODY()

public:
    ASoundZoneTrigger();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    FName ZoneName;

    UFUNCTION() void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
    UFUNCTION()
    void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
};
