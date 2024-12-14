#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

class UImage;
UCLASS()
class SINKDOWNPROJECT_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
    /*
    * meta = (BindWidget) is the name of a Widget Blueprint that tells Unreal Engine to automatically bind a Widget of the same name in the Widget Blueprint to this variable. 
    */
    UPROPERTY(meta = (BindWidget)) UImage* CrosshairTop;
    UPROPERTY(meta = (BindWidget)) UImage* CrosshairBottom;
    UPROPERTY(meta = (BindWidget)) UImage* CrosshairLeft;
    UPROPERTY(meta = (BindWidget)) UImage* CrosshairRight;

public:
    UFUNCTION(BlueprintCallable, Category = "Crosshair") void UpdateCrosshairSpread(float SpreadFactor);
};
