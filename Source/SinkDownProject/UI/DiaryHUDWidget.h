#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SinkDownProject/DataAsset/DiaryData.h"
#include "DiaryHUDWidget.generated.h"

class UImage;

UCLASS()
class SINKDOWNPROJECT_API UDiaryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup") UDiaryData* DiaryDataAsset;

    UPROPERTY(meta = (BindWidget)) UImage* DiaryIcon1;
    UPROPERTY(meta = (BindWidget)) UImage* DiaryIcon2;
    UPROPERTY(meta = (BindWidget)) UImage* DiaryIcon3;
    UPROPERTY(meta = (BindWidget)) UImage* DiaryIcon4;
    UPROPERTY(meta = (BindWidget)) UImage* DiaryIcon5;

private:
    void InitializeIcons();
    void UpdateIconVisibility(EDiaryPieceType PieceType);
    UImage* GetIconForPieceType(EDiaryPieceType PieceType);
	
    UFUNCTION() void OnDiaryPieceCollected(EDiaryPieceType PieceType);
};
