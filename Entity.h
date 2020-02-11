#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include "Transform.h"
#include "Mesh.h"

class Entity
{
	Transform transform;
	Mesh* mesh;

public:
	Entity(Mesh* mesh);
	~Entity();

	void Draw();

	Mesh* GetMesh();
	Transform* GetTransform();
};

