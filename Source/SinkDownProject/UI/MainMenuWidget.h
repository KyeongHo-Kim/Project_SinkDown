#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UImage;
class UWidgetAnimation;
UCLASS()
class SINKDOWNPROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget)) UImage* ManualImageWidget;
    UPROPERTY(meta = (BindWidget)) UButton* StartButton;
    UPROPERTY(meta = (BindWidget)) UButton* ManualButton;
    UPROPERTY(meta = (BindWidget)) UButton* ExitButton;
    UPROPERTY(meta = (BindWidget)) UButton* BackButton;


    UFUNCTION() void OnStartClicked();
    UFUNCTION() void OnManualClicked();
    UFUNCTION() void OnExitClicked();
    UFUNCTION() void OnBackClicked();

    UPROPERTY(Transient, meta = (BindWidgetAnim)) UWidgetAnimation* FadeInAnimation;
    UPROPERTY(Transient, meta = (BindWidgetAnim)) UWidgetAnimation* FadeOutAnimation;
};
