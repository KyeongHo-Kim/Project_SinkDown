#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamInterface.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
    Neutral,
    Player,
    Enemy
};

UINTERFACE(MinimalAPI)
class UTeamInterface : public UInterface
{
	GENERATED_BODY()
};

class SINKDOWNPROJECT_API ITeamInterface
{
	GENERATED_BODY()

public:
    virtual ETeam GetTeam() const = 0;
    virtual void SetTeam(ETeam NewTeam) = 0;
};
