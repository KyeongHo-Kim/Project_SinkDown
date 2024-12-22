#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

class UImage;
class UTextBlock;
class USoundBase;
class UAudioComponent;

USTRUCT(BlueprintType)
struct FCreditsEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Role;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Name;
};

UCLASS()
class SINKDOWNPROJECT_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* TitleImage;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* RoleText;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UTextBlock* NameText;

    // Set up credits
    UPROPERTY(EditAnywhere, Category = "Credits") TArray<FCreditsEntry> CreditsEntries;
    UPROPERTY(EditAnywhere, Category = "Credits") float DisplayTime = 2.0f;
    UPROPERTY(EditAnywhere, Category = "Credits") float FadeTime = 1.0f;


    UPROPERTY(EditDefaultsOnly, Category = "Sound") USoundBase* CreditsBGM;
    UPROPERTY(EditDefaultsOnly, Category = "Sound") float BGMFadeOutDuration = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Transition") float FadeOutToMenuDuration = 1.0f;
private:
    int32 CurrentEntryIndex;
    float CurrentDisplayTime;
    float CurrentFadeAlpha;
    bool bIsFadingIn;
    bool bIsFadingOut;

    void StartNextEntry();
    void UpdateFade(float DeltaTime);

    UAudioComponent* BGMAudio;
};
