// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Actor = GetAcceptableActor();
    if(Actor)
    {
        UE_LOG(LogTemp, Display, TEXT("Unlocking!!!"));
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if(Component)
        {
            Component->SetSimulatePhysics(false);
            Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        }
        if(Mover)
            Mover->SetShouldMove(true);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Relocking!!!"));
        if(Mover)
            Mover->SetShouldMove(false);
    }
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    for(AActor* Actor : Actors)
    {
        bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");
        if( HasAcceptableTag && !IsGrabbed)
        {
            return Actor;
        }
    }

    return nullptr;
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}