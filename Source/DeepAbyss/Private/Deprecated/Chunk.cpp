// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"

#include <stdexcept>

#include "Enums.h"
#include "ProceduralMeshComponent.h"
#include "DeepAbyss/FastNoiseLite.h"
#include "RealtimeMeshComponent.h"

// Sets default values
AChunk::AChunk()
{
	Blocks.SetNum(ChunkSize * ChunkSize * ChunkSize);
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	GenerateBlocks();

	GenerateMesh();

	ApplyMesh();
}

void AChunk::GenerateBlocks()
{
	const auto Location = GetActorLocation();

	for (int x = 0; x < ChunkSize; ++x)
	{
		for (int y = 0; y < ChunkSize; ++y)
		{
			// 1 unreal unit = 100 cm
			const float XPos =  (x * 100 + Location.X) / 100;
			const float YPos =  (y * 100 + Location.Y) / 100;

			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(XPos, YPos) + 1) * ChunkSize / 2),  0, ChunkSize);

			for (int z = 0; z < Height; ++z)
			{
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
			}

			for(int z = Height; z < ChunkSize; ++z)
			{
				Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
			}
		}
	}
}

void AChunk::GenerateMesh()
{
	for (int x = 0; x < ChunkSize; ++x)
	{
		for (int y = 0; y < ChunkSize; ++y)
		{
			for (int z = 0; z < ChunkSize; ++z)
			{
				if(Blocks[GetBlockIndex(x, y, z)] != EBlock::Air) // if current block is not air
				{
					const auto Position = FVector(x, y, z);

					// check all directions (6 blocks around)
					for(auto Direction : {EDirection::Forward, EDirection::Right, EDirection::Back, EDirection::Left, EDirection::Up, EDirection::Down})
					{
						if(CheckIfTransparent(GetPositionInDirection(Direction, Position)))
						{
							CreateFace(Direction, Position * 100);
						}
					}
				}				
			}
		}
	}
}

void AChunk::ApplyMesh() const
{
	Mesh->CreateMeshSection(0, VertexData, TriangleData, TArray<FVector>(),
		UVData, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

bool AChunk::CheckIfTransparent(FVector Position) const
{
	if(Position.X >= ChunkSize || Position.Y >= ChunkSize || Position.Z >= ChunkSize || Position.X < 0 || Position.Y < 0 || Position.Z < 0)
	{
		return true;
	}
	
	return Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)] == EBlock::Air;
}

void AChunk::CreateFace(EDirection Direction, FVector Position)
{
	VertexData.Append(GetFaceVertices(Direction, Position));
	UVData.Append({FVector2D(1, 1), FVector2D(1, 0), FVector2D(0, 0), FVector2D(0, 1)});
	TriangleData.Append({VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount});
	VertexCount += 4;
}

// get 4 vertices
TArray<FVector> AChunk::GetFaceVertices(EDirection Direction, FVector Position) const
{
	TArray<FVector> Vertices;

	for (int i = 0; i < 4; ++i)
	{
		Vertices.Add(BlockVertexData[BlockTriangleData[i + static_cast<int>(Direction) * 4]] * Scale + Position);
	}

	return Vertices;
}

FVector AChunk::GetPositionInDirection(EDirection Direction, FVector Position) const
{
	switch(Direction)
	{
	case EDirection::Forward: return Position + FVector::ForwardVector;
	case EDirection::Right: return Position + FVector::RightVector;
	case EDirection::Back: return Position + FVector::BackwardVector;
	case EDirection::Left: return Position + FVector::LeftVector;
	case EDirection::Up: return Position + FVector::UpVector;
	case EDirection::Down: return Position + FVector::DownVector;
	default: throw std::invalid_argument("Invalid Direction");
	}
}

int AChunk::GetBlockIndex(int X, int Y, int Z) const
{
	// flatten 3d position across X axis, Y axis and Z axis
	return Z * ChunkSize * ChunkSize + Y * ChunkSize + X;
}


