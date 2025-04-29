// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeTaskBase.h"
#include "TreeckoStateTreeTask_AutoTransitionAfterDelay.generated.h"


USTRUCT()
struct TREECKO_API FTreeckoStateTreeTask_AutoTransitionAfterDelay_InstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FStateTreeStateLink TargetState;

    UPROPERTY(EditAnywhere)
    float Delay = 0;

    mutable float TimeAcc = 0;
};


/**
 * 
 */
USTRUCT(DisplayName = "Auto Transition After Delay (Treecko)")
struct TREECKO_API FTreeckoStateTreeTask_AutoTransitionAfterDelay : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FTreeckoStateTreeTask_AutoTransitionAfterDelay_InstanceData;
    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); };

public:
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
                                           const FStateTreeTransitionResult& Transition) const override
    {
        return EStateTreeRunStatus::Running;
    };

    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override
    {
        const auto& InstanceData = Context.GetInstanceData(*this);
        InstanceData.TimeAcc += DeltaTime;

        if (InstanceData.TimeAcc >= InstanceData.Delay)
        {
            FStateTreeTransitionRequest Request;
            Request.TargetState = InstanceData.TargetState.StateHandle;
            Context.RequestTransition(Request);
        }

        return EStateTreeRunStatus::Running;
    };
};
