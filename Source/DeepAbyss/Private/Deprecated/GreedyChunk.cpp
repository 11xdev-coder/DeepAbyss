// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyChunk.h"

#include <stdexcept>

#include "Enums.h"
#include "ProceduralMeshComponent.h"
#include "DeepAbyss/FastNoiseLite.h"
#include "RealtimeMeshComponent.h"

// Sets default values
AGreedyChunk::AGreedyChunk()
{
	Blocks.SetNum(ChunkSize * ChunkSize * ChunkSize);
}

// Called when the game starts or when spawned
void AGreedyChunk::BeginPlay()
{
	Super::BeginPlay();

	GenerateBlocks();
}

__override void AGreedyChunk::GenerateBlocks()
{
	const auto Location = GetActorLocation();

	for (int x = 0; x < ChunkSize; ++x)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			// 1 unreal unit = 100 cm
			const float XPos =  (x * 100 + Location.X) / 100;
			const float YPos =  (y * 100 + Location.Y) / 100;

			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(XPos, YPos) + 1) * ChunkSize / 2),  0, ChunkSize);

			for (int z = 0; z < Height; z++)
			{
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
			}

			for(int z = Height; z < ChunkSize; z++)
			{
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
			}
		}
	}
}

void AGreedyChunk::ApplyMesh() const
{
	Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals,
		MeshData.UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

void AGreedyChunk::GenerateMesh()
{
	for (int axis = 0; axis < 3; ++axis)
	{
		const int Axis1 = (axis + 1) % 3;
		const int Axis2 = (axis + 2) % 3; // 2 perpendicular axes

		// limits along each axis
		const int MainAxisLimit = ChunkSize;
		int Axis1Limit = ChunkSize;
		int Axis2Limit = ChunkSize;

		auto DeltaAxis1 = FIntVector::ZeroValue;
		auto DeltaAxis2 = FIntVector::ZeroValue;

		// blocks iterating through
		auto ChunkIteration = FIntVector::ZeroValue;
		// which axis is active
		auto AxisMask = FIntVector::ZeroValue;
		AxisMask[axis] = 1;

		TArray<FMask> Mask;
		Mask.SetNum(Axis1Limit * Axis2Limit);

		// check each slice
		for (ChunkIteration[axis] = -1; ChunkIteration[axis] < MainAxisLimit; )
		{
			int MaskIteration = 0;

			// generate mask
			for(ChunkIteration[Axis2] = 0; ChunkIteration[Axis2] < Axis2Limit; ++ChunkIteration[Axis2])
			{
				for(ChunkIteration[Axis1] = 0; ChunkIteration[Axis1] < Axis1Limit; ++ChunkIteration[Axis1])
				{
					const auto CurrentBlock = GetBlock(ChunkIteration);
					const auto BlockToCompare = GetBlock(ChunkIteration + AxisMask);

					const bool IsCurrentBlockOpaque = CurrentBlock != EBlock::Air;
					const bool IsBlockToCompareOpaque = BlockToCompare != EBlock::Air;

					if(IsCurrentBlockOpaque == IsBlockToCompareOpaque)
					{
						Mask[MaskIteration++] = FMask {EBlock::Null, 0};
					}
					else if (IsCurrentBlockOpaque)
					{
						Mask[MaskIteration++] = FMask {CurrentBlock, 1};
					}
					else
					{
						Mask[MaskIteration++] = FMask {BlockToCompare, -1};
					}
				}
			}

			++ChunkIteration[axis];
			MaskIteration = 0;

			// generate mesh from mask
			for(int j = 0; j < Axis2Limit; ++j)
			{
				for (int i = 0; i < Axis1Limit;)
				{
					if(Mask[MaskIteration].Normal != 0)
					{
						const auto CurrentMask = Mask[MaskIteration];
						ChunkIteration[Axis1] = i;
						ChunkIteration[Axis2] = j;

						int Width;
						// get maximum width of the quad
						for(Width = 1; i + Width < Axis1Limit && CompareMask(Mask[MaskIteration + Width], CurrentMask); ++Width)
						{
							
						}

						int Height;
						bool Done = false;
						for (Height = 1; j + Height < Axis2Limit; ++Height)
						{
							for (int k = 0; k < Width; ++k)
							{
								if (CompareMask(Mask[MaskIteration + k + Height * Axis1Limit], CurrentMask)) continue;

								Done = true;
								break;
							}

							if (Done) break;
						}

						DeltaAxis1[Axis1] = Width;
						DeltaAxis2[Axis2] = Height;

						CreateQuad(CurrentMask, AxisMask,
							Width, Height,
							ChunkIteration,
							ChunkIteration + DeltaAxis1,
							ChunkIteration + DeltaAxis2,
							ChunkIteration + DeltaAxis1 + DeltaAxis2);

						// clean up
						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;

						for (int l = 0; l < Height; ++l)
						{
							for (int k = 0; k < Width; ++k)
							{
								Mask[MaskIteration + k + l * Axis1Limit] = FMask {EBlock::Null, 0};
							}
						}

						i += Width;
						MaskIteration += Width;
					}
					else
					{
						i++;
						MaskIteration++;
					}
				}
			}
		}
	}
}

void AGreedyChunk::CreateQuad(FMask Mask, FIntVector AxisMask,
	int Width, int Height,
	FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4)
{
	const auto Normal = FVector(AxisMask * Mask.Normal);

	MeshData.Vertices.Append({
		FVector(V1) * 100,
		FVector(V2) * 100,
		FVector(V3) * 100,
		FVector(V4) * 100
	});

	MeshData.Triangles.Append({
		VertexCount,
		VertexCount + 2 + Mask.Normal,
		VertexCount + 2 - Mask.Normal,
		VertexCount + 3,
		VertexCount + 1 - Mask.Normal,
		VertexCount + 1 + Mask.Normal
	});

	MeshData.Normals.Append({
		Normal,
		Normal,
		Normal,
		Normal
	});

	if (Normal.X == 1 || Normal.X == -1)
	{
		MeshData.UVs.Append({
			FVector2D(Width, Height),
			FVector2D(0, Height),
			FVector2D(Width, 0),
			FVector2D(0, 0),
		});
	}
	else
	{
		MeshData.UVs.Append({
			FVector2D(Height, Width),
			FVector2D(Height, 0),
			FVector2D(0, Width),
			FVector2D(0, 0),
		});
	}

	VertexCount += 4;
}


int AGreedyChunk::GetBlockIndex(int X, int Y, int Z) const
{
	return Z * ChunkSize * ChunkSize + Y * ChunkSize + X;
}

EBlock AGreedyChunk::GetBlock(FIntVector Index) const
{
	if(Index.X >= ChunkSize || Index.Y >= ChunkSize || Index.Z >= ChunkSize || Index.X < 0 || Index.Y < 0 || Index.Z < 0)
	{
		return EBlock::Air;
	}

	return Blocks[GetBlockIndex(Index.X, Index.Y, Index.Z)];
}

bool AGreedyChunk::CompareMask(FMask M1, FMask M2) const
{
	return M1.Normal == M2.Normal && M1.Block == M2.Block;
}





