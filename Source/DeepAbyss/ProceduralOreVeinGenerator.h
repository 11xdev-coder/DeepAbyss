// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

#include "ProceduralOreVeinGenerator.generated.h"

UCLASS()
class DEEPABYSS_API AProceduralOreVeinGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralOreVeinGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UProceduralMeshComponent* RealtimeMesh;

	void GenerateOreVeins();

	void ConnectCubes(TArray<FVector>& Vertices, TArray<int32>& Triangles, FVector Location1, FVector Location2, float Size);

	void GenerateCube(TArray<FVector>& Vertices, TArray<int32>& Triangles, FVector Location, float Size);
};
