// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoxelFunctionLibrary.generated.h"


struct FMarchingCubesData;
/**
 * 
 */
UCLASS()
class UVoxelFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
	static void GenerateSphereVoxelValues(float Radius, FVector Center, TArray<float>& OutSphereVoxelValues, int ChunkSize, float VoxelSize);

	static void IntegrateSphereVoxelValuesIntoChunk(TArray<float>& SphereVoxelValues, FVector Center, AMarchingCubesChunk* Chunk, float Radius, float VoxelSize);
	
	UFUNCTION(BlueprintCallable, Category = "Carving")
	static void CarveRadiusAtLocation(float Radius, FVector Location, TArray<AMarchingCubesChunk*> OverlappingChunks, UMaterialInstance* CarveMaterial);

	UFUNCTION(BlueprintPure, Category = "Array")
	static bool IsArrayEqualToElement(TArray<float> Arr, float Element);	

	// marching cubes data
	const static int VertexOffset[8][3];

	const static int EdgeConnection[12][2];        

	const static float EdgeDirection[12][3];

	const static int CubeEdgeFlags[256];

	const static int TriangleConnectionTable[256][16];
};
