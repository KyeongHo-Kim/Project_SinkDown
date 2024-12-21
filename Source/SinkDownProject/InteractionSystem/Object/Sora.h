#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SinkDownProject/InteractionSystem/InteractionInterface.h"
#include "Sora.generated.h"

class UWidgetComponent;
class UAnimMontage;

UCLASS()
class SINKDOWNPROJECT_API ASora : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ASora();

protected:

	virtual void BeginPlay() override;
	

protected:
	virtual void InteractionWithMe() override;

	virtual void ShowInteractionWidget() override;
	virtual void HideInteractionWidget() override;

	UPROPERTY(EditAnywhere) UWidgetComponent* InteractionWidget;
	UPROPERTY(EditAnywhere, Category = "Animation") UAnimMontage* InteractionMontage;
};
