#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class Entity
{
	Transform transform;
	Mesh* mesh;
	Material* material;

public:
	Entity(Mesh* mesh, Material* material);
	~Entity();

	void Draw();

	Mesh* GetMesh();
	Transform* GetTransform();
	Material* GetMaterial();
};

