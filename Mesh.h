#pragma once
#include "Vertex.h"
#include "DXCore.h"
#include <wrl/client.h>

class Mesh
{
private:
	// Buffers to hold geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	// int to hold the number of indices in the index buffer
	int numberOfIndices;
	
	Mesh(
		Vertex* objects, 
		int numberOfVertices, 
		int* indices, 
		int numberOfIndices, 
		Microsoft::WRL::ComPtr<ID3D11Device> device);
	~Mesh();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();

public:

};

