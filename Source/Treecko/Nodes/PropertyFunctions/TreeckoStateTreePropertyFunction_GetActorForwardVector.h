// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeExecutionContext.h"
#include "StateTreePropertyFunctionBase.h"
#include "TreeckoStateTreePropertyFunction_GetActorForwardVector.generated.h"

USTRUCT()
struct TREECKO_API FTreeckoStateTreePropertyFunction_GetActorForwardVector_InstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TObjectPtr<AActor> Actor;

    UPROPERTY(EditAnywhere, Category=Output)
    FVector ForwardVector = FVector::ZeroVector;
};

/**
 * 
 */
USTRUCT(DisplayName = "Get Actor Forward Vector (Treecko)")
struct TREECKO_API FTreeckoStateTreePropertyFunction_GetActorForwardVector : public FStateTreePropertyFunctionCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FTreeckoStateTreePropertyFunction_GetActorForwardVector_InstanceData;

    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

    virtual void Execute(FStateTreeExecutionContext& Context) const override
    {
        auto& [Actor,ForwardVector] = Context.GetInstanceData(*this);
        ForwardVector = Actor->GetActorForwardVector();
    };
};
