// Fill out your copyright notice in the Description page of Project Settings.


#include "TunnelGenerator.h"
#include "CustomProceduralMeshComponent.h"
#include "Engine/Engine.h"

// Sets default values
ATunnelGenerator::ATunnelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATunnelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	UCustomProceduralMeshComponent* Mesh = NewObject<UCustomProceduralMeshComponent>(this);
	Mesh->GenerateLine(StartPoint, EndPoint, 1.5f);
}

// Called every frame
void ATunnelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


