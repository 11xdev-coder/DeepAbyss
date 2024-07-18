// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubesChunk.h"

#include "ProceduralMeshComponent.h"
#include "DeepAbyss/FastNoiseLite.h"

AMarchingCubesChunk::AMarchingCubesChunk()
{
	VoxelValues.SetNum((ChunkSize + 1) * (ChunkSize + 1) * (ChunkSize + 1));
}

void AMarchingCubesChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AMarchingCubesChunk::GenerateMesh()
{
	// triangle order
	if (SurfaceLevel > 0.0f)
	{
		TriangleOrder[0] = 0;
		TriangleOrder[1] = 1;
		TriangleOrder[2] = 2;
	}
	else
	{
		TriangleOrder[0] = 2;
		TriangleOrder[1] = 1;
		TriangleOrder[2] = 0;
	}

	// march
	float Cube[8]; // 8 vertices

	for (int X = 0; X < ChunkSize; ++X)
	{
		for (int Y = 0; Y < ChunkSize; ++Y)
		{
			for (int Z = 0; Z < ChunkSize; ++Z)
			{
				for (int i = 0; i < 8; ++i) // each vertex
				{
					Cube[i] = VoxelValues[GetVoxelIndex(X + VertexOffset[i][0],Y + VertexOffset[i][1],Z + VertexOffset[i][2])];
				}

				March(X,Y,Z, Cube);
			}
		}
	}
}

void AMarchingCubesChunk::GenerateHeightMap()
{
	const auto ActorLocation = GetActorLocation() / 100;

	for (int x = 0; x <= ChunkSize; ++x)
	{
		for (int y = 0; y <= ChunkSize; ++y)
		{
			for (int z = 0; z <= ChunkSize; ++z)
			{
				VoxelValues[GetVoxelIndex(x, y, z)] = Noise->GetNoise(x + ActorLocation.X,
																		y + ActorLocation.Y, z + ActorLocation.Z);
			}
		}
	}	
}

void AMarchingCubesChunk::March(int X, int Y, int Z, const float CubeVoxelValues[8])
{
	int VertexMask = 0;
	FVector EdgeVertices[12];

	// compute vertex mask
	// check 8 cube values
	for (int i = 0; i < 8; ++i)
	{
		if (CubeVoxelValues[i] <= SurfaceLevel) VertexMask |= 1 << i;
	}

	// edge mask
	const int EdgeMask = CubeEdgeFlags[VertexMask];
	if (EdgeMask == 0) return; // nothing to draw

	// find intersection points
	for (int i = 0; i < 12; ++i)
	{
		if((EdgeMask & 1 << i) != 0)
		{
			const float Offset = Interpolate ? GetInterpolationOffset(CubeVoxelValues[EdgeConnection[i][0]],
				CubeVoxelValues[EdgeConnection[i][1]]) : 0.5f;

			EdgeVertices[i].X = X + (VertexOffset[EdgeConnection[i][0]][0] + Offset * EdgeDirection[i][0]);
			EdgeVertices[i].Y = Y + (VertexOffset[EdgeConnection[i][0]][1] + Offset * EdgeDirection[i][1]);
			EdgeVertices[i].Z = Z + (VertexOffset[EdgeConnection[i][0]][2] + Offset * EdgeDirection[i][2]);
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		if (TriangleConnectionTable[VertexMask][3 * i] < 0) break; // if we reach -1 (nothing to draw) , break

		auto V1 = EdgeVertices[TriangleConnectionTable[VertexMask][3 * i]] * 100;
		auto V2 = EdgeVertices[TriangleConnectionTable[VertexMask][3 * i + 1]] * 100;
		auto V3 = EdgeVertices[TriangleConnectionTable[VertexMask][3 * i + 2]] * 100;

		auto Normal = FVector::CrossProduct(V2 - V1, V3 - V1);
		auto Color = FColor::MakeRandomColor();

		Normal.Normalize();

		MeshData.Vertices.Append({V1, V2, V3});

		MeshData.Triangles.Append({
			VertexCount + TriangleOrder[0],
			VertexCount + TriangleOrder[1],
			VertexCount + TriangleOrder[2],
		});

		MeshData.Normals.Append({
			Normal,
			Normal,
			Normal
		});

		MeshData.Colors.Append({
			Color,
			Color,
			Color
		});

		VertexCount += 3;
	}
}

float AMarchingCubesChunk::GetInterpolationOffset(float V1, float V2) const
{
	const float Delta = V2 - V1;

	return Delta == 0.0f ? SurfaceLevel : (SurfaceLevel - V1) / Delta;
}

int AMarchingCubesChunk::GetVoxelIndex(int X, int Y, int Z) const
{
	return Z * (ChunkSize + 1) * (ChunkSize + 1) + Y * (ChunkSize + 1) + X;
}





