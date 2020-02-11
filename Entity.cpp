#include "Entity.h"

Entity::Entity(Mesh* mesh)
{
	this->mesh = mesh;
}

Entity::~Entity()
{
}

void Entity::Draw()
{

}

Mesh* Entity::GetMesh()
{
	return mesh;
}

Transform* Entity::GetTransform()
{
	return &transform;
}
