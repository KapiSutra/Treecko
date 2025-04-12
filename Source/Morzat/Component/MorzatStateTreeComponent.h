// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "MorzatStateTreeComponent.generated.h"

class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct MORZAT_API FMorzatStateTreeComponentContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AActor> Owner;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<AActor> Avatar;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<USkeletalMeshComponent> MeshComponent;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMorzatActorContextUpdatedDelegate);

UCLASS(ClassGroup=(Morzat), meta=(BlueprintSpawnableComponent))
class MORZAT_API UMorzatStateTreeComponent : public UStateTreeComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UMorzatStateTreeComponent();
    virtual TSubclassOf<UStateTreeSchema> GetSchema() const override;

    virtual void BeginPlay() override;
    virtual void InitializeComponent() override;

protected:
    virtual bool SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors = false) override;

public:
    UPROPERTY(BlueprintReadOnly)
    FMorzatStateTreeComponentContext ActorContext;

    // Should Be Called Each Time After AbilitySystemComponent::InitAbilityActorInfo
    UFUNCTION(BlueprintCallable)
    void UpdateActorContext();

    UPROPERTY(BlueprintAssignable)
    FMorzatActorContextUpdatedDelegate PostActorContextUpdated;

    virtual UAbilitySystemComponent* SearchAbilitySystemComponent();
};
