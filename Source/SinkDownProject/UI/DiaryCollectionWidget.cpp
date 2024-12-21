#include "SinkDownProject/UI/DiaryCollectionWidget.h"
#include "SinkDownProject/GameLevels/DiarySubsystem.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"

void UDiaryCollectionWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UpdateCollection();
}

void UDiaryCollectionWidget::UpdateCollection()
{
    if (!DiaryDataAsset) return;

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UDiarySubsystem* DiarySubsystem = GameInstance->GetSubsystem<UDiarySubsystem>())
        {
            // Update the visibility of each image widget
            UpdateDiaryPieceVisibility(Diary_1, EDiaryPieceType::Piece1);
            UpdateDiaryPieceVisibility(Diary_2, EDiaryPieceType::Piece2);
            UpdateDiaryPieceVisibility(Diary_3, EDiaryPieceType::Piece3);
            UpdateDiaryPieceVisibility(Diary_4, EDiaryPieceType::Piece4);
            UpdateDiaryPieceVisibility(Diary_5, EDiaryPieceType::Piece5);
        }
    }
}

void UDiaryCollectionWidget::UpdateDiaryPieceVisibility(UImage* DiaryImage, EDiaryPieceType PieceType)
{
    if (!DiaryImage) return;

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UDiarySubsystem* DiarySubsystem = GameInstance->GetSubsystem<UDiarySubsystem>())
        {
            bool bIsCollected = DiarySubsystem->HasCollectedPiece(PieceType);
            DiaryImage->SetOpacity(bIsCollected ? 1.0f : 0);
        }
    }
}

void UDiaryCollectionWidget::ToggleVisibility()
{
    if (IsVisible())
    {
        RemoveFromParent();
    }
    else
    {
        if (OpenSound)
        {
            UGameplayStatics::PlaySound2D(this, OpenSound);
        }

        AddToViewport();
        UpdateCollection();
        PlayOpenAnimation();
    }
}

void UDiaryCollectionWidget::PlayOpenAnimation()
{
    if (FadeIn)
    {
        PlayAnimation(FadeIn);
    }
}

void UDiaryCollectionWidget::PlayCloseAnimation()
{
    if (FadeOut)
    {
        PlayAnimation(FadeOut, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);

        // Remove the widget at the end of the animation
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDiaryCollectionWidget::OnCloseAnimationFinished, FadeOut->GetEndTime(), false);
    }
    else
    {
        RemoveFromParent();
    }
}

void UDiaryCollectionWidget::OnCloseAnimationFinished()
{
    RemoveFromParent();
}