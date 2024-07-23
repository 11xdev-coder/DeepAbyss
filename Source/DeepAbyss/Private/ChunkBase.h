// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FChunkMeshData.h"
#include "RealtimeMeshActor.h"
#include "GameFramework/Actor.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Minerals/MineralBase.h"
#include "ChunkBase.generated.h"

class URealtimeMeshComponent;
class FastNoiseLite;
class UVoxelFunctionLibrary;
class URealtimeMeshSimple;
class UProceduralMeshComponent;
class UDynamicMeshComponent;

UCLASS()
class AChunkBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkBase();

	FIntVector ChunkSize = FIntVector(256, 256, 256);

	float Frequency;
	TObjectPtr<UMaterialInstance> Material;

	AMineralBase* DefaultMineral;
	
	FChunkMeshData MeshData;

	virtual void GenerateMesh();
	
	void ApplyMesh();

	void ClearMeshData();

	TObjectPtr<UDynamicMeshComponent> DynamicMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GenerateHeightMap();

	TObjectPtr<UProceduralMeshComponent> Mesh;
	
	FDynamicMesh3 DynamicMeshDataHolder;
	TObjectPtr<FastNoiseLite> Noise;
	TObjectPtr<UVoxelFunctionLibrary> Lib;
	
	int VertexCount = 0;	
	
};
