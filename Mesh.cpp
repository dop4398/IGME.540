#include "Mesh.h"

Mesh::Mesh(Vertex* objects, int numberOfVertices, int* indices, int numberOfIndices, Microsoft::WRL::ComPtr<ID3D11Device> device)
{
}

Mesh::~Mesh()
{
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer()
{
	return indexBuffer;
}

int Mesh::GetIndexCount()
{
	return numberOfIndices;
}
