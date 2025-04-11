// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "MorzatStateTreeComponent.generated.h"


UCLASS(ClassGroup=(Morzat), meta=(BlueprintSpawnableComponent))
class MORZAT_API UMorzatStateTreeComponent : public UStateTreeComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UMorzatStateTreeComponent();

    virtual TSubclassOf<UStateTreeSchema> GetSchema() const override;

protected:
    virtual bool SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors = false) override;
};
