// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelFunctionLibrary.h"

#include "MarchingCubesChunk.h"
#include "ProceduralMeshComponent.h"

void UVoxelFunctionLibrary::CarveRadiusAtLocation(float Radius, FVector Location, TArray<AMarchingCubesChunk*> OverlappingChunks,
	UMaterialInstance* CarveMaterial)
{
	for (AMarchingCubesChunk* Chunk : OverlappingChunks)
	{
		if (!Chunk) continue;

		// Perform carving in a separate thread
		Async(EAsyncExecution::Thread, [=]()
		{
			FVector ChunkLocation = Chunk->GetActorLocation();
			FVector LocalLocation = Location - ChunkLocation;
			FIntVector ChunkSize = Chunk->ChunkSize;
			float VoxelSize = 100.0f;
			int SubChunkSize = 16; // Size of sub-chunks

			bool Modified = false;

			for (int x = 0; x <= ChunkSize.X; x += SubChunkSize)
			{
				for (int y = 0; y <= ChunkSize.Y; y += SubChunkSize)
				{
					for (int z = 0; z <= ChunkSize.Z; z += SubChunkSize)
					{
						bool SubChunkModified = false;

						for (int dx = 0; dx < SubChunkSize; ++dx)
						{
							for (int dy = 0; dy < SubChunkSize; ++dy)
							{
								for (int dz = 0; dz < SubChunkSize; ++dz)
								{
									int vx = x + dx;
									int vy = y + dy;
									int vz = z + dz;
									if (vx > ChunkSize.X || vy > ChunkSize.Y || vz > ChunkSize.Z) continue;

									FVector VoxelPosition = FVector(vx, vy, vz) * VoxelSize;
									if (FVector::Dist(VoxelPosition, LocalLocation) < Radius)
									{
										int VoxelIndex = Chunk->GetVoxelIndex(vx, vy, vz);
										Chunk->VoxelValues[VoxelIndex] = 0; // remove voxels that are in radius
										SubChunkModified = true;
										Modified = true;
									}
								}
							}
						}

						if (SubChunkModified)
						{
							// Update the sub-chunk
							Async(EAsyncExecution::TaskGraphMainThread, [=]()
							{
								Chunk->GenerateMesh(); // Ideally, only regenerate the affected sub-chunk
								Chunk->ApplyMesh();
							});
						}
					}
				}
			}
		});
	}
}

bool UVoxelFunctionLibrary::IsArrayEqualToElement(TArray<float> Arr, float Element)
{
	for(float E : Arr)
	{
		if (E != Element) return false;
	}

	return true;
}

