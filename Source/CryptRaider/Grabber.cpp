// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Component is null"));
	}

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) {
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDist;

		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Release() {
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) {
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
		
	}
}

void UGrabber::Grab() {
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
		return;

	FHitResult HitResult;
	bool HasHit = GetGrabbable(HitResult);
	
	if (HasHit) {
		UPrimitiveComponent *HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent,
		 NAME_None, HitResult.ImpactPoint, GetComponentRotation());
		HitResult.GetActor()->Tags.Add("Grabbed");
	} else {
		UE_LOG(LogTemp, Display, TEXT("Miss"));
	}
}

bool UGrabber::GetGrabbable(FHitResult &HitResult) const {
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDist;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End,
	 FQuat::Identity, ECC_GameTraceChannel2, Sphere);
}

UPhysicsHandleComponent *UGrabber::GetPhysicsHandle() {
	UPhysicsHandleComponent *PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	return PhysicsHandle;
}