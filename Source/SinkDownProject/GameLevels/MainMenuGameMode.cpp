#include "SinkDownProject/GameLevels/MainMenuGameMode.h"
#include "SinkDownProject/UI/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

AMainMenuGameMode::AMainMenuGameMode()
{
}

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Set the fade-in start state
    if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
    {
        CameraManager->SetManualCameraFade(1.0f, FLinearColor::Black, false);
    }

    if (MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();

            if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
            {
                PC->SetShowMouseCursor(true);
                FInputModeUIOnly InputMode;
                PC->SetInputMode(InputMode);
            }
        }
    }
}
