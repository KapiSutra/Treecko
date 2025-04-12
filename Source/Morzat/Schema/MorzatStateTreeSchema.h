// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeSchema.h"
#include "Components/StateTreeComponent.h"
#include "MorzatStateTreeSchema.generated.h"

class UStateTreeComponent;

namespace Morzat
{
    struct FStateTreeContextDataNames
    {
        const static FName ContextOwner,
                           ContextAvatar,
                           ContextStateTreeComponent,
                           ContextMeshComponent,
                           ContextAbilitySystemComponent;
    };
}

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, meta=(CommonSchema), DisplayName = "Morzat StateTree")
class MORZAT_API UMorzatStateTreeSchema : public UStateTreeSchema
{
    GENERATED_BODY()

public:
    UMorzatStateTreeSchema();

protected:
    virtual TConstArrayView<FStateTreeExternalDataDesc> GetContextDataDescs() const override;
    virtual bool IsStructAllowed(const UScriptStruct* InScriptStruct) const override;
    virtual bool IsClassAllowed(const UClass* InScriptStruct) const override;
    virtual bool IsExternalItemAllowed(const UStruct& InStruct) const override;
    
    virtual void PostLoad() override;
    //

    UPROPERTY()
    TArray<FStateTreeExternalDataDesc> ContextDataDescs;
    //

public:
    UPROPERTY(EditAnywhere, NoClear)
    TSubclassOf<AActor> OwnerType = AActor::StaticClass();

    UPROPERTY(EditAnywhere, NoClear)
    TSubclassOf<AActor> AvatarType = AActor::StaticClass();

    UPROPERTY(EditAnywhere, NoClear)
    TSubclassOf<UStateTreeComponent> StateTreeComponentType;

protected:
#if WITH_EDITOR
    virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};
