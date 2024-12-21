#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SinkDownProject/DataAsset/DiaryData.h"
#include "DiaryCollectionWidget.generated.h"

class UCanvasPanel;
class UImage;
class UWidgetAnimation;
class USoundBase;

UCLASS()
class SINKDOWNPROJECT_API UDiaryCollectionWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
    UDiaryData* DiaryDataAsset;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* DiaryCanvas;

    UPROPERTY(meta = (BindWidget)) UImage* Diary_1;
    UPROPERTY(meta = (BindWidget)) UImage* Diary_2;
    UPROPERTY(meta = (BindWidget)) UImage* Diary_3;
    UPROPERTY(meta = (BindWidget)) UImage* Diary_4;
    UPROPERTY(meta = (BindWidget)) UImage* Diary_5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound") USoundBase* OpenSound;

    // Animation
    UPROPERTY(Transient, meta = (BindWidgetAnim)) UWidgetAnimation* FadeIn;
    UPROPERTY(Transient, meta = (BindWidgetAnim))  UWidgetAnimation* FadeOut;


public:
    void ToggleVisibility();
    void UpdateCollection();

private:
    void UpdateDiaryPieceVisibility(UImage* DiaryImage, EDiaryPieceType PieceType);


    void PlayOpenAnimation();
    void PlayCloseAnimation();
    UFUNCTION() void OnCloseAnimationFinished();
};