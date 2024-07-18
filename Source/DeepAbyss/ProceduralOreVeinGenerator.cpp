// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralOreVeinGenerator.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProceduralOreVeinGenerator::AProceduralOreVeinGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	RealtimeMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
	RootComponent = RealtimeMesh;
}

// Called when the game starts or when spawned
void AProceduralOreVeinGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateOreVeins();
}

// Called every frame
void AProceduralOreVeinGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralOreVeinGenerator::GenerateOreVeins()
{
    int CubeNum = 10;
    float CubeSize = 100.0f;

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> CubeLocations;

    // Generate a grid of cubes
    for (int i = 0; i < CubeNum; ++i)
    {
        FVector Location = UKismetMathLibrary::RandomPointInBoundingBox(FVector(0,0,0), FVector(1000, 1000, 1000));
        CubeLocations.Add(Location);
    }

    // Generate cubes and connect their vertices
    for (int i = 0; i < CubeNum; ++i)
    {
        GenerateCube(Vertices, Triangles, CubeLocations[i], CubeSize);

        // Connect to the next cube
        if (i < CubeNum - 1)
        {
            ConnectCubes(Vertices, Triangles, CubeLocations[i], CubeLocations[i + 1], CubeSize);
        }
        // Connect the last cube to the first cube
        else
        {
            ConnectCubes(Vertices, Triangles, CubeLocations[i], CubeLocations[0], CubeSize);
        }
    }

    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    // Create the mesh section
    RealtimeMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}

void AProceduralOreVeinGenerator::GenerateCube(TArray<FVector>& Vertices, TArray<int32>& Triangles, FVector Location, float Size)
{
    int32 VertexIndex = Vertices.Num();

    // Define vertices for a cube
    Vertices.Add(Location + FVector(0, 0, 0));
    Vertices.Add(Location + FVector(Size, 0, 0));
    Vertices.Add(Location + FVector(Size, Size, 0));
    Vertices.Add(Location + FVector(0, Size, 0));
    Vertices.Add(Location + FVector(0, 0, Size));
    Vertices.Add(Location + FVector(Size, 0, Size));
    Vertices.Add(Location + FVector(Size, Size, Size));
    Vertices.Add(Location + FVector(0, Size, Size));

    // Define triangles for each face of the cube
    // Bottom face
    Triangles.Append({VertexIndex + 0, VertexIndex + 1, VertexIndex + 2});
    Triangles.Append({VertexIndex + 0, VertexIndex + 2, VertexIndex + 3});
    // Top face
    Triangles.Append({VertexIndex + 4, VertexIndex + 6, VertexIndex + 5});
    Triangles.Append({VertexIndex + 4, VertexIndex + 7, VertexIndex + 6});
    // Front face
    Triangles.Append({VertexIndex + 0, VertexIndex + 4, VertexIndex + 1});
    Triangles.Append({VertexIndex + 1, VertexIndex + 4, VertexIndex + 5});
    // Right face
    Triangles.Append({VertexIndex + 1, VertexIndex + 5, VertexIndex + 2});
    Triangles.Append({VertexIndex + 2, VertexIndex + 5, VertexIndex + 6});
    // Back face
    Triangles.Append({VertexIndex + 2, VertexIndex + 6, VertexIndex + 3});
    Triangles.Append({VertexIndex + 3, VertexIndex + 6, VertexIndex + 7});
    // Left face
    Triangles.Append({VertexIndex + 3, VertexIndex + 7, VertexIndex + 0});
    Triangles.Append({VertexIndex + 0, VertexIndex + 7, VertexIndex + 4});
}

void AProceduralOreVeinGenerator::ConnectCubes(TArray<FVector>& Vertices, TArray<int32>& Triangles, FVector Location1, FVector Location2, float Size)
{
    // Calculate the offset to connect the corners
    FVector Offset = (Location2 - Location1) / 2.0f;

    int32 VertexIndex = Vertices.Num();

    // Define vertices for connecting corners
    Vertices.Add(Location1 + FVector(0, 0, 0) + Offset);
    Vertices.Add(Location1 + FVector(Size, 0, 0) + Offset);
    Vertices.Add(Location1 + FVector(Size, Size, 0) + Offset);
    Vertices.Add(Location1 + FVector(0, Size, 0) + Offset);
    Vertices.Add(Location1 + FVector(0, 0, Size) + Offset);
    Vertices.Add(Location1 + FVector(Size, 0, Size) + Offset);
    Vertices.Add(Location1 + FVector(Size, Size, Size) + Offset);
    Vertices.Add(Location1 + FVector(0, Size, Size) + Offset);

    // Define triangles for connecting corners
    // Bottom face
    Triangles.Append({VertexIndex + 0, VertexIndex + 1, VertexIndex + 2});
    Triangles.Append({VertexIndex + 0, VertexIndex + 2, VertexIndex + 3});
    // Top face
    Triangles.Append({VertexIndex + 4, VertexIndex + 6, VertexIndex + 5});
    Triangles.Append({VertexIndex + 4, VertexIndex + 7, VertexIndex + 6});
    // Front face
    Triangles.Append({VertexIndex + 0, VertexIndex + 4, VertexIndex + 1});
    Triangles.Append({VertexIndex + 1, VertexIndex + 4, VertexIndex + 5});
    // Right face
    Triangles.Append({VertexIndex + 1, VertexIndex + 5, VertexIndex + 2});
    Triangles.Append({VertexIndex + 2, VertexIndex + 5, VertexIndex + 6});
    // Back face
    Triangles.Append({VertexIndex + 2, VertexIndex + 6, VertexIndex + 3});
    Triangles.Append({VertexIndex + 3, VertexIndex + 6, VertexIndex + 7});
    // Left face
    Triangles.Append({VertexIndex + 3, VertexIndex + 7, VertexIndex + 0});
    Triangles.Append({VertexIndex + 0, VertexIndex + 7, VertexIndex + 4});
}
