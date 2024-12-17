#pragma once

#include "CoreMinimal.h"
#include "SinkDownProject/InteractionSystem/Object/InteractiveObjectBase.h"
#include "PowerButton.generated.h"

class UStaticMeshComponent;
class AStaticMeshActor;
class UTimelineComponent;
class UCurveFloat;
class UMaterialInstanceDynamic;
class UPasswordInputWidget;
class AMatrixWorldActivator;

UCLASS()
class SINKDOWNPROJECT_API APowerButton : public AInteractiveObjectBase
{
    GENERATED_BODY()

public:
    APowerButton();

protected:
    virtual void BeginPlay() override;
    virtual void OnInteract() override;

private:
    UPROPERTY(EditAnywhere, Category = "Mesh") UStaticMeshComponent* PowerButtonMesh;

    UPROPERTY(EditInstanceOnly, Category = "Power Effect") AStaticMeshActor* ScreenPlane;
    UPROPERTY() UTimelineComponent* FadeTimeline;
    UPROPERTY(EditDefaultsOnly, Category = "Power Effect") UCurveFloat* FadeCurve;
    UPROPERTY() UMaterialInstanceDynamic* CachedDynamicMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UPasswordInputWidget> PasswordWidgetClass;
    UPROPERTY() UPasswordInputWidget* PasswordWidget;


    UPROPERTY(EditInstanceOnly, Category = "Matrix World") AMatrixWorldActivator* TargetActivator;
    UPROPERTY(EditAnywhere, Category = "Matrix World") FVector LocationOffset;
    UPROPERTY(EditDefaultsOnly, Category = "Matrix World")UCurveFloat* MovementCurve;
    UPROPERTY() UTimelineComponent* MovementTimeline;
    FVector StartLocation;
    FVector TargetLocation;

    void InitializeDynamicMaterial();
    UFUNCTION() void OnFadeUpdate(float Value);

    UFUNCTION() void OnPasswordResult(bool bSuccess);
    void ShowPasswordWidget();
    void ResetScreen();

    UFUNCTION() void OnMovementUpdate(float Value);
    void MoveCharacterToActivator();
};  