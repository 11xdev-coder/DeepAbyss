// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkBase.h"
#include "ChunkWorld.h"
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
	UFUNCTION(BlueprintCallable, Category = "Carving")
	static void CarveRadiusAtLocation(AChunkWorld* World, float Radius, FVector Location, TArray<AChunkBase*> OverlappingChunks, 
									bool ChangeColor, FColor CarveColor);
	
	UFUNCTION(BlueprintPure, Category = "Array")
	static bool IsArrayEqualToElement(TArray<float> Arr, float Element);	

	// marching cubes data
	const static int VertexOffset[8][3];

	const static int EdgeConnection[12][2];        

	const static float EdgeDirection[12][3];

	const static int CubeEdgeFlags[256];

	const static int TriangleConnectionTable[256][16];
};
