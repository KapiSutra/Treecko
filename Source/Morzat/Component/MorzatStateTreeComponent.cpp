// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "MorzatStateTreeComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/PlayerState.h"
#include "Morzat/Schema/MorzatStateTreeSchema.h"


// Sets default values for this component's properties
UMorzatStateTreeComponent::UMorzatStateTreeComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    bWantsInitializeComponent = true;

    // ...
}

TSubclassOf<UStateTreeSchema> UMorzatStateTreeComponent::GetSchema() const
{
    return UMorzatStateTreeSchema::StaticClass();
}

void UMorzatStateTreeComponent::BeginPlay()
{
    Super::BeginPlay();
    UpdateActorContext();
}

void UMorzatStateTreeComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

bool UMorzatStateTreeComponent::SetContextRequirements(FStateTreeExecutionContext& Context, const bool bLogErrors)
{
    auto Result = Super::SetContextRequirements(Context, bLogErrors);

    Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextOwner, ActorContext.Owner.Get());
    Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextAvatar, ActorContext.Avatar.Get());
    Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextStateTreeComponent, this);
    Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextMeshComponent,
                                 ActorContext.MeshComponent.Get());
    Context.SetContextDataByName(Morzat::FStateTreeContextDataNames::ContextAbilitySystemComponent,
                                 ActorContext.AbilitySystemComponent.Get());

    const auto* Schema = CastChecked<UMorzatStateTreeSchema>(Context.GetStateTree()->GetSchema());

    Result &= ActorContext.Owner && ActorContext.Owner->IsA(Schema->OwnerType);
    Result &= ActorContext.Avatar && ActorContext.Avatar->IsA(Schema->AvatarType);
    Result &= this->IsA(Schema->StateTreeComponentType);
    Result &= !!ActorContext.MeshComponent;
    Result &= !!ActorContext.AbilitySystemComponent;

    return Result;
}

void UMorzatStateTreeComponent::UpdateActorContext()
{
    ActorContext.Owner = GetOwner();
    ActorContext.AbilitySystemComponent = SearchAbilitySystemComponent();

    if (ensure(ActorContext.AbilitySystemComponent))
    {
        ActorContext.Avatar = ActorContext.AbilitySystemComponent->GetAvatarActor();
        ActorContext.MeshComponent = ActorContext.AbilitySystemComponent->AbilityActorInfo->
                                                  SkeletalMeshComponent.Get();
    }
}

UAbilitySystemComponent* UMorzatStateTreeComponent::SearchAbilitySystemComponent()
{
    const auto Owner = GetOwner();

    UAbilitySystemComponent* Result = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);

    if (Result)
    {
        return Result;
    }

    if (const auto PawnOwner = Cast<APawn>(Owner))
    {
        if (const auto Controller = PawnOwner->GetController())
        {
            Result = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Controller);
            if (Result)
            {
                return Result;
            }

            if (const auto PC = Cast<APlayerController>(Controller))
            {
                if (const auto PS = PC->PlayerState)
                {
                    Result = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PS);
                    if (Result)
                    {
                        return Result;
                    }
                }
            }
        }
    }

    if (const auto ControllerOwner = Cast<AController>(Owner))
    {
        if (const auto Pawn = ControllerOwner->GetPawn())
        {
            Result = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn);
            if (Result)
            {
                return Result;
            }
        }

        if (const auto PCOwner = Cast<APlayerController>(ControllerOwner))
        {
            if (const auto PS = PCOwner->PlayerState)
            {
                Result = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PS);
                if (Result)
                {
                    return Result;
                }
            }
        }
    }


    return Result;
}
