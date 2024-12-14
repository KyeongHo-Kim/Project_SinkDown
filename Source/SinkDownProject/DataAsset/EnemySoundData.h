// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemySoundData.generated.h"

/**
 * 
 */
UCLASS()
class SINKDOWNPROJECT_API UEnemySoundData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Detection")
    USoundBase* DetectionSound;

    UPROPERTY(EditAnywhere, Category = "Detection")
    float DetectionVolume = 1.0f;
};