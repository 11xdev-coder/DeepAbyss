// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkBase.h"
#include "ChunkWorld.h"
#include "MarchingCubesChunk.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Minerals/MineralBase.h"
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
	UFUNCTION(BlueprintCallable, Category = "Carving")
	static void CarveRadiusAtLocation(AChunkWorld* World, float Radius, FVector Location, TArray<AChunkBase*> OverlappingChunks, 
									bool ChangeMineral, TSubclassOf<AMineralBase> Mineral);

	UFUNCTION(BlueprintPure, Category = "Voxels")
	static AMineralBase* GetMineralAt(AChunkBase* Chunk, FVector Location);

	UFUNCTION(BlueprintPure, Category = "Voxels")
	static bool CanMineAt(AChunkBase* Chunk, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Voxels")
	static bool MineRadius(AMarchingCubesChunk* MarchingCubesChunk, FVector MineLocation, float MineRadius);

	UFUNCTION(BlueprintPure, Category = "Mineral")
	static FName GetMineralName(AMineralBase* Mineral);
	
	UFUNCTION(BlueprintPure, Category = "Array")
	static bool IsArrayEqualToElement(TArray<float> Arr, float Element);
	
	static FVector GetVoxelGridCoords(FVector ChunkLocation, FVector HitLocation);
	
	// marching cubes data
	const static int VertexOffset[8][3];

	const static int EdgeConnection[12][2];        

	const static float EdgeDirection[12][3];

	const static int CubeEdgeFlags[256];

	const static int TriangleConnectionTable[256][16];
};
