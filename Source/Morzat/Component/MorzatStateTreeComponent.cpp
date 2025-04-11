// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "MorzatStateTreeComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MorzatContextProviderInterface.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/PlayerState.h"
#include "Morzat/Schema/MorzatStateTreeSchema.h"


// Sets default values for this component's properties
UMorzatStateTreeComponent::UMorzatStateTreeComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

TSubclassOf<UStateTreeSchema> UMorzatStateTreeComponent::GetSchema() const
{
    return UMorzatStateTreeSchema::StaticClass();
}

bool UMorzatStateTreeComponent::SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors)
{
    auto Result = Super::SetContextRequirements(Context, bLogErrors);
    const auto Schema = CastChecked<UMorzatStateTreeSchema>(Context.GetStateTree()->GetSchema());

    if (Schema && Result && Context.IsValid())
    {
        const auto Owner = GetOwner();
        Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextOwner, Owner);

        AActor* Avatar = nullptr;
        USkeletalMeshComponent* MeshComponent;
        UAbilitySystemComponent* AbilitySystemComponent =
            UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);

        if (!AbilitySystemComponent)
        {
            if (const auto ControllerOwner = Cast<AController>(Owner))
            {
                AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
                    ControllerOwner->GetPawn());

                if (!AbilitySystemComponent)
                {
                    if (const auto PlayerControllerOwner = Cast<APlayerController>(Owner))
                    {
                        AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
                            PlayerControllerOwner->PlayerState);
                    }
                }
            }
        }


        if (Owner->Implements<UMorzatContextProviderInterface>())
        {
            Avatar = IMorzatContextProviderInterface::Execute_GetAvatar(Owner);
        }
        else if (AbilitySystemComponent)
        {
            Avatar = AbilitySystemComponent->GetAvatarActor();
        }

        if (!Avatar)
        {
            Avatar = Owner;
        }

        if (AbilitySystemComponent)
        {
            MeshComponent = AbilitySystemComponent->AbilityActorInfo->SkeletalMeshComponent.Get();
        }
        else
        {
            MeshComponent = Avatar->FindComponentByClass<USkeletalMeshComponent>();
        }

        Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextAvatar, Avatar);
        Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextStateTreeComponent, this);
        Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextMeshComponent, MeshComponent);
        Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextAbilitySystemComponent,
                                     AbilitySystemComponent);

        Result &= Owner->IsA(Schema->OwnerType);
        Result &= Avatar->IsA(Schema->AvatarType);
        Result &= Avatar->IsA(Schema->StateTreeComponentType);
    }
    else
    {
        Result &= false;
    }


    return Result;
}
