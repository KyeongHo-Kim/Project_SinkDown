#include "SinkDownProject/UI/CreditsWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

    CurrentEntryIndex = -1;
    CurrentDisplayTime = 0.0f;
    CurrentFadeAlpha = 0.0f;
    bIsFadingIn = false;
    bIsFadingOut = false;

    if (RoleText && NameText)
    {
        RoleText->SetOpacity(0.0f);
        NameText->SetOpacity(0.0f);
    }

    if (CreditsBGM)
    {
        BGMAudio = UGameplayStatics::SpawnSound2D(this, CreditsBGM);
    }

    StartNextEntry();
}

void UCreditsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bIsFadingIn || bIsFadingOut)
    {
        UpdateFade(InDeltaTime);
    }
    else if (CurrentEntryIndex >= 0)
    {
        CurrentDisplayTime += InDeltaTime;
        if (CurrentDisplayTime >= DisplayTime)
        {
            // Start fading out the current entry
            bIsFadingOut = true;
            CurrentFadeAlpha = 1.0f;
        }
    }
}

void UCreditsWidget::StartNextEntry()
{
    CurrentEntryIndex++;
    if (CurrentEntryIndex >= CreditsEntries.Num())
    {
        // BGM fade out
        if (BGMAudio)
        {
            BGMAudio->FadeOut(BGMFadeOutDuration, 0.0f);
        }

        // Fade out the screen
        APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        if (CameraManager)
        {
            CameraManager->StartCameraFade(0.0f, 1.0f, FadeOutToMenuDuration, FLinearColor::Black, true, true);
        }

        // Switch levels after a delay
       /* FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle,
            [this]()
            {
                UGameplayStatics::OpenLevel(this, TEXT("/Game/Levels/MainMenuMap"));

            },
            BGMFadeOutDuration, false);
        return;*/
        FTimerHandle TimerHandle;
        TWeakObjectPtr<UCreditsWidget> WeakThis(this);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle,
            [WeakThis]()
            {
                if (WeakThis.IsValid())
                {
                    UGameplayStatics::OpenLevel(WeakThis.Get()->GetWorld(), TEXT("/Game/Levels/MainMenuMap"));
                }
            },
            BGMFadeOutDuration, false);
        return;
    }

    // Set up a new entry
    if (RoleText && NameText)
    {
        RoleText->SetText(FText::FromString(CreditsEntries[CurrentEntryIndex].Role));
        NameText->SetText(FText::FromString(CreditsEntries[CurrentEntryIndex].Name));
    }

    // Start fading in
    bIsFadingIn = true;
    CurrentFadeAlpha = 0.0f;
    CurrentDisplayTime = 0.0f;
}


void UCreditsWidget::UpdateFade(float DeltaTime)
{
    if (bIsFadingIn)
    {
        CurrentFadeAlpha += DeltaTime / FadeTime;
        if (CurrentFadeAlpha >= 1.0f)
        {
            CurrentFadeAlpha = 1.0f;
            bIsFadingIn = false;
        }
    }
    else if (bIsFadingOut)
    {
        CurrentFadeAlpha -= DeltaTime / FadeTime;
        if (CurrentFadeAlpha <= 0.0f)
        {
            CurrentFadeAlpha = 0.0f;
            bIsFadingOut = false;
            StartNextEntry();
        }
    }

    // Update transparency
    if (RoleText && NameText)
    {
        RoleText->SetOpacity(CurrentFadeAlpha);
        NameText->SetOpacity(CurrentFadeAlpha);
    }
}