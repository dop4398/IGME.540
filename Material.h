#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h>
#include <Windows.h>
#include "SimpleShader.h"

class Material
{
	DirectX::XMFLOAT4 colorTint;

	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;

public:
	Material(
		SimplePixelShader* ps,
		SimpleVertexShader* vs,
		DirectX::XMFLOAT4 colorTint);

	void SetColorTint(DirectX::XMFLOAT4 tint);

	DirectX::XMFLOAT4 GetColorTint();
	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
};

