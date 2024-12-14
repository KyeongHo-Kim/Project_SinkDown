#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MugunghwaGameEffectsData.generated.h"


UCLASS()
class SINKDOWNPROJECT_API UMugunghwaGameEffectsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Sound") USoundBase* MugunghwaNarration;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") USoundBase* CorrectKeyPressSound;  
	UPROPERTY(EditDefaultsOnly, Category = "Sound") USoundBase* WrongKeyPressSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") USoundBase* WASDGameSuccessSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") USoundBase* EndMugunghwaGameSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound") USoundBase* MugunghwaOpenDoorSound;
};
