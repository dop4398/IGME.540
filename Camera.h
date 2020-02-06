#pragma once
#include "Game.h"

class Camera
{
public:
	Camera(float x, float y, float z, float aspectRatio, float mouseLookSpeed);
	~Camera();

	// Updating
	void Update();
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	// Matrices
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

