#include "SinkDownProject/SubGame/MatrixWorldGame/PowerButton.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SinkDownProject/UI/PasswordInputWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SinkDownProject/SubGame/MatrixWorldGame/MatrixWorldActivator.h"
#include "GameFramework/Character.h"

APowerButton::APowerButton()
{
    PowerButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerButtonMesh"));
    PowerButtonMesh->SetupAttachment(RootComponent);

    FadeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FadeTimeline"));
    MovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MovementTimeline"));
}

void APowerButton::BeginPlay()
{
    Super::BeginPlay();

    InitializeDynamicMaterial();

    if (FadeCurve)
    {
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindDynamic(this, &APowerButton::OnFadeUpdate);
        FadeTimeline->AddInterpFloat(FadeCurve, TimelineCallback);
    }

    if (MovementCurve) 
    {
        FOnTimelineFloat MovementCallback;
        MovementCallback.BindDynamic(this, &APowerButton::OnMovementUpdate);
        MovementTimeline->AddInterpFloat(MovementCurve, MovementCallback);
    }
}

void APowerButton::OnInteract()
{
    if (!CachedDynamicMaterial)
    {
        InitializeDynamicMaterial();
    }

    if (ScreenPlane && FadeTimeline && FadeCurve && CachedDynamicMaterial)
    {
        FadeTimeline->PlayFromStart();
        ShowPasswordWidget();
    }
}

void APowerButton::InitializeDynamicMaterial()
{
    if (ScreenPlane && !CachedDynamicMaterial)
    {
        if (UStaticMeshComponent* MeshComp = ScreenPlane->GetStaticMeshComponent())
        {
            if (UMaterialInterface* Material = MeshComp->GetMaterial(0))
            {
                CachedDynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
                MeshComp->SetMaterial(0, CachedDynamicMaterial);
            }
        }
    }
}

void APowerButton::OnFadeUpdate(float Value)
{
    if (CachedDynamicMaterial)
    {
        CachedDynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), 1.0f - Value);
    }
}   


void APowerButton::ShowPasswordWidget()
{
    if (PasswordWidgetClass && !PasswordWidget)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (PlayerController)
        {
            PasswordWidget = CreateWidget<UPasswordInputWidget>(PlayerController, PasswordWidgetClass);
            if (PasswordWidget)
            {
                PasswordWidget->OnPasswordResult.AddDynamic(this, &APowerButton::OnPasswordResult);
                PasswordWidget->AddToViewport();
            }
        }
    }
}

void APowerButton::OnPasswordResult(bool bSuccess)
{
    if (bSuccess && TargetActivator)
    {
        MoveCharacterToActivator();
    }

    if (!bSuccess)
    {
        ResetScreen();
    }

    if (PasswordWidget)
    {
        PasswordWidget->RemoveFromParent();
        PasswordWidget = nullptr;
    }
}

void APowerButton::ResetScreen()
{
    if (CachedDynamicMaterial)
    {
        CachedDynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), 1.0f);
    }
}

void APowerButton::MoveCharacterToActivator()
{
    if (!TargetActivator)
    {
        return;
    }

    if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
    {
        StartLocation = PlayerCharacter->GetActorLocation();
        TargetLocation = TargetActivator->GetActorLocation() + LocationOffset;

        if (MovementTimeline && MovementCurve)
        {
            MovementTimeline->PlayFromStart();
        }
    }
}

void APowerButton::OnMovementUpdate(float Value)
{
    if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
    {
        FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Value);
        PlayerCharacter->SetActorLocation(NewLocation);
    }
}