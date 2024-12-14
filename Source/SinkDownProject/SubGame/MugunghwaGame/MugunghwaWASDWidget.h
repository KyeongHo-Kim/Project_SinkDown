#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MugunghwaWASDWidget.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;

UCLASS()
class SINKDOWNPROJECT_API UMugunghwaWASDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void UpdateKeySequence(const TArray<FKey>& KeySequence, int32 CurrentIndex);
    void ShowWidget();
    void HideWidget();
    void UpdateTimeRemaining(float RemainingTime);
    void SetMaxTime(float MaxTime) { TotalTime = MaxTime; }

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget)) UProgressBar* TimeRemainingBar;
    UPROPERTY(meta = (BindWidget)) UTextBlock* KeySequenceText;

    UPROPERTY(EditAnywhere, Category = "UI Colors")
    FLinearColor StartColor = FLinearColor(1.0f, 0.8f, 0.6f, 1.0f); 

    UPROPERTY(EditAnywhere, Category = "UI Colors")
    FLinearColor EndColor = FLinearColor(0.8f, 0.0f, 0.0f, 1.0f);

    float TotalTime;
};