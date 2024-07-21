// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubesChunk.h"

#include "ProceduralMeshComponent.h"
#include "RealtimeMeshSimple.h"
#include "VoxelFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "DeepAbyss/FastNoiseLite.h"
#include "DynamicMesh/DynamicMesh3.h"

AMarchingCubesChunk::AMarchingCubesChunk()
{
	VoxelValues.SetNum((ChunkSize.X + 1) * (ChunkSize.Y + 1) * (ChunkSize.Z + 1));
	VoxelColors.SetNum((ChunkSize.X + 1) * (ChunkSize.Y + 1) * (ChunkSize.Z + 1));
}

void AMarchingCubesChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AMarchingCubesChunk::GenerateMesh()
{
	ClearMeshData();
	
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

	for (int X = 0; X < ChunkSize.X; ++X)
	{
		for (int Y = 0; Y < ChunkSize.Y; ++Y)
		{
			for (int Z = 0; Z < ChunkSize.Z; ++Z)
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

	for (int x = 0; x <= ChunkSize.X; ++x)
	{
		for (int y = 0; y <= ChunkSize.Y; ++y)
		{
			for (int z = 0; z <= ChunkSize.Z; ++z)
			{
				VoxelValues[GetVoxelIndex(x, y, z)] = Noise->GetNoise(x + ActorLocation.X,
																		y + ActorLocation.Y, z + ActorLocation.Z);
				VoxelColors[GetVoxelIndex(x, y, z)] = DefaultTintColor; // by default use default color
			}
		}
	}
}


void AMarchingCubesChunk::March(int X, int Y, int Z, const float CubeVoxelValues[8])
{
	int VertexMask = 0;
	FVector EdgeVertices[12];
	FLinearColor EdgeColors[12];

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

			FVector VertexPosition(
			   X + (VertexOffset[EdgeConnection[i][0]][0] + Offset * EdgeDirection[i][0]),
			   Y + (VertexOffset[EdgeConnection[i][0]][1] + Offset * EdgeDirection[i][1]),
			   Z + (VertexOffset[EdgeConnection[i][0]][2] + Offset * EdgeDirection[i][2])
		    );

			EdgeVertices[i] = VertexPosition;

			// slowly interp between colors
			FColor ColorA = VoxelColors[GetVoxelIndex(X + VertexOffset[EdgeConnection[i][0]][0], Y + VertexOffset[EdgeConnection[i][0]][1], Z + VertexOffset[EdgeConnection[i][0]][2])];
            FColor ColorB = VoxelColors[GetVoxelIndex(X + VertexOffset[EdgeConnection[i][1]][0], Y + VertexOffset[EdgeConnection[i][1]][1], Z + VertexOffset[EdgeConnection[i][1]][2])];
			EdgeColors[i] = FLinearColor::LerpUsingHSV(FLinearColor(ColorA), FLinearColor(ColorB), Offset);
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		if (TriangleConnectionTable[VertexMask][3 * i] < 0) break; // if we reach -1 (nothing to draw) , break

		auto V1 = EdgeVertices[TriangleConnectionTable[VertexMask][3 * i]] * 100;
		auto V2 = EdgeVertices[TriangleConnectionTable[VertexMask][3 * i + 1]] * 100;
		auto V3 = EdgeVertices[TriangleConnectionTable[VertexMask][3 * i + 2]] * 100;

		auto Normal = FVector::CrossProduct(V2 - V1, V3 - V1);
		
		// get interpolated colors
		FColor Color1 = EdgeColors[TriangleConnectionTable[VertexMask][3 * i]].ToFColor(true);
		FColor Color2 = EdgeColors[TriangleConnectionTable[VertexMask][3 * i + 1]].ToFColor(true);
		FColor Color3 = EdgeColors[TriangleConnectionTable[VertexMask][3 * i + 2]].ToFColor(true);

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
			Color1,
			Color2,
			Color3
		});

		
		// vertices
		int32 VIndex1 = DynamicMeshDataHolder.AppendVertex(V1);
		int32 VIndex2 = DynamicMeshDataHolder.AppendVertex(V2);
		int32 VIndex3 = DynamicMeshDataHolder.AppendVertex(V3);

		
		DynamicMeshDataHolder.AppendTriangle(VertexCount + TriangleOrder[0],
			VertexCount + TriangleOrder[1],
			VertexCount + TriangleOrder[2]);

		DynamicMeshDataHolder.SetVertexNormal(VIndex1, (FVector3f)Normal);
		DynamicMeshDataHolder.SetVertexNormal(VIndex2, (FVector3f)Normal);
		DynamicMeshDataHolder.SetVertexNormal(VIndex3, (FVector3f)Normal);

		VertexCount += 3;
	}
}

// void AMarchingCubesChunk::OnGenerateMesh_Implementation()
// {
// 	URealtimeMeshSimple* InitializedMesh = GetRealtimeMeshComponent()->InitializeRealtimeMesh<URealtimeMeshSimple>();
// 	FRealtimeMeshStreamSet StreamSet;
//
// 	TRealtimeMeshBuilderLocal<int16, FPackedNormal, FVector2DHalf, 1> Builder(StreamSet);
//
// 	Builder.EnableTangents();
// 	Builder.EnableTexCoords();
// 	Builder.EnableColors();
//
// 	Builder.AddVertex(FVector3f(V1.X, V1.Y, V1.Z)).SetNormal(FVector3f(Normal.X, Normal.Y, Normal.Z)).SetColor(Color);
// 	Builder.AddVertex(FVector3f(V2.X, V2.Y, V2.Z)).SetNormal(FVector3f(Normal.X, Normal.Y, Normal.Z)).SetColor(Color);
// 	Builder.AddVertex(FVector3f(V3.X, V3.Y, V3.Z)).SetNormal(FVector3f(Normal.X, Normal.Y, Normal.Z)).SetColor(Color);
// 	
// 	Builder.AddTriangle(
// 		VertexCount + TriangleOrder[0],
// 		VertexCount + TriangleOrder[1],
// 		VertexCount + TriangleOrder[2]
// 	);
// 		
// 	VertexCount += 3;
//
// 	InitializedMesh->SetupMaterialSlot(0, "Primary");
//
// 	const FRealtimeMeshSectionGroupKey GroupKey = FRealtimeMeshSectionGroupKey::Create(0, FName("Cave"));
// 	const FRealtimeMeshSectionKey PolyGroupSectionKey = FRealtimeMeshSectionKey::CreateForPolyGroup(GroupKey, 0);
// 	InitializedMesh->CreateSectionGroup(GroupKey, StreamSet);
// 	InitializedMesh->UpdateSectionConfig(PolyGroupSectionKey, FRealtimeMeshSectionConfig(ERealtimeMeshSectionDrawType::Static, 0));
// }

float AMarchingCubesChunk::GetInterpolationOffset(float V1, float V2) const
{
	const float Delta = V2 - V1;

	return Delta == 0.0f ? SurfaceLevel : (SurfaceLevel - V1) / Delta;
}

int AMarchingCubesChunk::GetVoxelIndex(int X, int Y, int Z) const
{
	return (Z * (ChunkSize.X + 1) * (ChunkSize.Y + 1)) + Y * (ChunkSize.X + 1) + X;
}





