// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FChunkMeshData.h"
#include "GameFramework/Actor.h"
#include "ChunkBase.generated.h"

class UProceduralMeshComponent;
class FastNoiseLite;

UCLASS()
class AChunkBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkBase();

	UPROPERTY(EditAnywhere, Category = "Chunk Settings")
	int ChunkSize = 32;

	float Frequency;
	TObjectPtr<UMaterialInterface> Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GenerateHeightMap();

	virtual void GenerateMesh();

	TObjectPtr<UProceduralMeshComponent> Mesh;
	TObjectPtr<FastNoiseLite> Noise;
	FChunkMeshData MeshData;
	int VertexCount = 0;	

private:	
	void ApplyMesh() const;
};
