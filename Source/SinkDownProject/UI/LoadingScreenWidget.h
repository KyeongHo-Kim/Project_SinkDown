#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"


class UTextBlock;
class UMediaPlayer;
class UMediaSource;
class UMediaTexture;
class UImage;
class UCircularThrobber;

UCLASS()
class SINKDOWNPROJECT_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UPROPERTY(meta = (BindWidget)) UCircularThrobber* LoadingProgressBar;
    UPROPERTY(meta = (BindWidget)) UTextBlock* LoadingText;

    UPROPERTY(meta = (BindWidget))
    UImage* BackgroundVideo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Media")
    UMediaPlayer* MediaPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    UMediaSource* VideoSource;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    UMediaTexture* MediaTexture;

public:
    void UpdateProgress(float Progress);
    void SetLoadingText(const FText& NewText);
	
    UFUNCTION(BlueprintCallable, Category = "Loading")
    void PlayLoadingVideo();

    UFUNCTION(BlueprintCallable, Category = "Loading")
    void StopLoadingVideo();
};
