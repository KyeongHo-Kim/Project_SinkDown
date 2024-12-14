#include "SinkDownProject/UI/LoadingScreenWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Components/Image.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSource.h"
#include "Components/CircularThrobber.h"

void ULoadingScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Set initial state
    if (LoadingProgressBar)
    {
        LoadingProgressBar->SetVisibility(ESlateVisibility::Visible);
    }

    if (LoadingText)
    {
        LoadingText->SetText(FText::FromString(TEXT("Loading...")));
    }


    // Initialize media player
    if (!MediaPlayer)
    {
        MediaPlayer = NewObject<UMediaPlayer>(this);
    }

    if (!MediaTexture)
    {
        MediaTexture = NewObject<UMediaTexture>(this);
        MediaTexture->SetMediaPlayer(MediaPlayer);
        MediaTexture->UpdateResource();
    }

    // Set up the background video image
    if (BackgroundVideo && MediaTexture)
    {
        BackgroundVideo->SetBrushResourceObject(MediaTexture);
    }

    // Auto play the video if source is set
    if (VideoSource)
    {
        PlayLoadingVideo();
    }
}


void ULoadingScreenWidget::NativeDestruct()
{
    Super::NativeDestruct();

    if (MediaPlayer)
    {
        MediaPlayer->Close();
    }
}

void ULoadingScreenWidget::UpdateProgress(float Progress)
{
    if (LoadingText)
    {
        FString ProgressText = FString::Printf(TEXT("Loading..."));
        LoadingText->SetText(FText::FromString(ProgressText));
    }
}

void ULoadingScreenWidget::SetLoadingText(const FText& NewText)
{
    if (LoadingText)
    {
        LoadingText->SetText(NewText);
    }
}

void ULoadingScreenWidget::PlayLoadingVideo()
{
    if (MediaPlayer && VideoSource)
    {
        MediaPlayer->OpenSource(VideoSource);
        MediaPlayer->Play();
        MediaPlayer->SetLooping(true);
    }
}

void ULoadingScreenWidget::StopLoadingVideo()
{
    if (MediaPlayer)
    {
        MediaPlayer->Close();
    }
}