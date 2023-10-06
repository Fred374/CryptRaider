// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay() {
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (int i = 0; i < Actors.Num(); i++) {
		UE_LOG(LogTemp, Display, TEXT("%s"), *Actors[i]->GetActorNameOrLabel());
		if (Actors[i]->ActorHasTag(TriggerTag) && !Actors[i]->ActorHasTag("Grabbed")) {
			if (Mover != nullptr)
				Mover->SetShouldMove(true);
			break;
		} else {
			if (Mover != nullptr)
				Mover->SetShouldMove(false);
		}
	}

}

void UTriggerComponent::SetMover(UMover *NewMover) {
	Mover = NewMover;
}