// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "MorzatStateTreeSchema.h"

#include "AbilitySystemComponent.h"
#include "StateTreeConditionBase.h"
#include "StateTreeConsiderationBase.h"
#include "StateTreeEvaluatorBase.h"
#include "StateTreeExecutionTypes.h"
#include "StateTreePropertyFunctionBase.h"
#include "StateTreeTaskBase.h"

namespace Morzat
{
    const FName FStateTreeContextDataNames::ContextOwner = TEXT("Owner");
    const FName FStateTreeContextDataNames::ContextAvatar = TEXT("Avatar");
    const FName FStateTreeContextDataNames::ContextStateTreeComponent = TEXT("State Tree Component");
    const FName FStateTreeContextDataNames::ContextMeshComponent = TEXT("Mesh Component");
    const FName FStateTreeContextDataNames::ContextAbilitySystemComponent = TEXT("Ability System Component");
}


UMorzatStateTreeSchema::UMorzatStateTreeSchema(): StateTreeComponentType(UStateTreeComponent::StaticClass())
{
    ContextDataDescs.Append({
        {
            Morzat::FStateTreeContextDataNames::ContextOwner,
            AActor::StaticClass(),
            FGuid("D98379D5C1B14EA8B81B766E04661E75")
        },
        {
            Morzat::FStateTreeContextDataNames::ContextAvatar,
            AActor::StaticClass(),
            FGuid("FF4251599EC7443080EE3C4CDEC11CCF")
        },
        {
            Morzat::FStateTreeContextDataNames::ContextStateTreeComponent,
            UStateTreeComponent::StaticClass(),
            FGuid("3DABE82CEB204BEF9111AE0800B1647E")
        },
        {
            Morzat::FStateTreeContextDataNames::ContextMeshComponent,
            USkeletalMeshComponent::StaticClass(),
            FGuid("C151B3D68FFE4318B6C0383A102D4B97")
        },
        {
            Morzat::FStateTreeContextDataNames::ContextAbilitySystemComponent,
            UAbilitySystemComponent::StaticClass(),
            FGuid("8D6D1CC1D0B943378837DC8F7ED2C765")
        },

    });
}

TConstArrayView<FStateTreeExternalDataDesc> UMorzatStateTreeSchema::GetContextDataDescs() const
{
    return ContextDataDescs;
}

bool UMorzatStateTreeSchema::IsStructAllowed(const UScriptStruct* InScriptStruct) const
{
    return InScriptStruct->IsChildOf(FStateTreeConditionCommonBase::StaticStruct())
        || InScriptStruct->IsChildOf(FStateTreeEvaluatorCommonBase::StaticStruct())
        || InScriptStruct->IsChildOf(FStateTreeTaskCommonBase::StaticStruct())
        || InScriptStruct->IsChildOf(FStateTreeConsiderationCommonBase::StaticStruct())
        || InScriptStruct->IsChildOf(FStateTreePropertyFunctionCommonBase::StaticStruct());
}

bool UMorzatStateTreeSchema::IsClassAllowed(const UClass* InScriptStruct) const
{
    return IsChildOfBlueprintBase(InScriptStruct);
}

bool UMorzatStateTreeSchema::IsExternalItemAllowed(const UStruct& InStruct) const
{
    return true;
}

void UMorzatStateTreeSchema::PostLoad()
{
    Super::PostLoad();
}

#if WITH_EDITOR
void UMorzatStateTreeSchema::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
    Super::PostEditChangeChainProperty(PropertyChangedEvent);

    const FProperty* Property = PropertyChangedEvent.Property;

    if (Property)
    {
        if (Property->GetOwnerClass() == StaticClass()
            && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UMorzatStateTreeSchema, OwnerType))
        {
            ContextDataDescs[0].Struct = OwnerType.Get();
        }
        if (Property->GetOwnerClass() == StaticClass()
            && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UMorzatStateTreeSchema, AvatarType))
        {
            ContextDataDescs[1].Struct = AvatarType.Get();
        }
        if (Property->GetOwnerClass() == StaticClass()
            && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UMorzatStateTreeSchema, StateTreeComponentType))
        {
            ContextDataDescs[2].Struct = StateTreeComponentType.Get();
        }
    }
}
#endif
