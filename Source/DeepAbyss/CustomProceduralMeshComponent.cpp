// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomProceduralMeshComponent.h"

#include "ProceduralMeshComponent.h"

void UCustomProceduralMeshComponent::GenerateLine(FVector StartPoint, FVector EndPoint, float Thickness)
{
	if(StartPoint == EndPoint) return;

	// Setup vertices
	int VerticesCount = 2;
	
	TArray<FVector> VertexPositions;
	VertexPositions.SetNum(VerticesCount); // Resize array
	VertexPositions[0] = StartPoint; // Starting point

	// Calculate second vertex position using linear interpolation, considering Thickness
	float LineDistance = (EndPoint - StartPoint).Size(); // Size() returns length
	FVector Direction = EndPoint - StartPoint;
	int VerticesPerLineSegmentSide = 2 * Thickness / LineDistance + 1; // Including both sides of the line

	// Linear interpolation
	for (int i = 0; i < VerticesPerLineSegmentSide; ++i)
	{
		FVector InterpolatedVertexPosition(StartPoint);

		float AlphaFactor = static_cast<float>(i) / VerticesPerLineSegmentSide - 0.5f;
		InterpolatedVertexPosition += Direction * (AlphaFactor * Thickness + LineDistance * (1 - AlphaFactor));

		VertexPositions[VerticesCount++] = InterpolatedVertexPosition;
	}

	// Setup Triangles
	TArray<int> TriangleIndices;
	int IndexOffset = 0;
	TriangleIndices.SetNum(VerticesCount / 3);

	for (int i = 0; i < VerticesPerLineSegmentSide; ++i)
	{
		int CurrentVertexIndex = IndexOffset + i;

		TriangleIndices[TriangleIndices.Num() - 1] = CurrentVertexIndex;
		if((CurrentVertexIndex + 1) % (VerticesCount / 3) == 0)
		{
			TriangleIndices[TriangleIndices.Num() - 1] = CurrentVertexIndex + VerticesPerLineSegmentSide;
		}

		++IndexOffset;
	}
	
	// Create custom material instance
	UMaterialInstanceDynamic* LineColorMaterial = NewObject<UMaterialInstanceDynamic>(this);

	// Apply Red color
	FLinearColor Red(1, 0, 0);
	LineColorMaterial->SetVectorParameterValue(FName(TEXT("BaseColor")), Red);

	UProceduralMeshComponent* Mesh = NewObject<UProceduralMeshComponent>(this);
	Mesh->CreateMeshSection(0, VertexPositions, TriangleIndices, TArray<FVector>(),
		TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}
