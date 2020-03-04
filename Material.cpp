#include "Material.h"

Material::Material(SimplePixelShader* ps, SimpleVertexShader* vs, DirectX::XMFLOAT4 colorTint)
{
	pixelShader = ps;
	vertexShader = vs;
	this->colorTint = colorTint;
	specularIntensity = 0.5f;
}

Material::Material(SimplePixelShader* ps, SimpleVertexShader* vs, DirectX::XMFLOAT4 colorTint, float specularIntensity)
{
	pixelShader = ps;
	vertexShader = vs;
	this->colorTint = colorTint;
	this->specularIntensity = specularIntensity;
}

void Material::SetColorTint(DirectX::XMFLOAT4 tint)
{
	colorTint = tint;
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
	return colorTint;
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

float Material::GetSpecularIntensity()
{
	return specularIntensity;
}
