// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if(PhysicsHandle == nullptr)
		return;

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if(PhysicsHandle == nullptr)
		return;

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGarbDistance;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10.0f, 10, FColor::Blue, false, 5.0f);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);

	if(HasHit)
	{
		UE_LOG(LogTemp, Display, TEXT("Grab Something!!!"));
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None, 
			HitResult.ImpactPoint, 
			GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Release Grabber!!!"));

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if(PhysicsHandle == nullptr)
		return;

	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle()
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr)
		UE_LOG(LogTemp, Display, TEXT("You Need PhysicsHandle!!!"));
	return Result;
}