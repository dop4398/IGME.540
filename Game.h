#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Material.h"
#include "Lights.h"
#include <vector>
#include "TerrainMesh.h"


class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();

	// Matrices
	DirectX::XMFLOAT4X4 worldMatrix;

	// Entities
	std::vector<Entity*> entities = std::vector<Entity*>();
	//bullets
	std::vector<Entity*> bulletList = std::vector<Entity*>();
	Mesh* bulletMesh;
	Material* bulletMaterial;
	const float GRAVITY = -9.8f;

	// User input and entity swapping
	int currentEntity;
	bool prevTab;
	bool prevLButton;

	Camera* camera;
	
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Mode, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Shaders and shader-related constructs
	/*SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;*/

	SimplePixelShader* pixelShaderNormalMap;
	SimpleVertexShader* vertexShaderNormalMap;

	SimplePixelShader* currentPS;
	SimpleVertexShader* currentVS;

	// Materials for Assignment 5
	std::vector<Material*> materials;

	// Lights
	std::vector<DirectionalLight> dLights = std::vector<DirectionalLight>();
	std::vector<PointLight> pLights = std::vector<PointLight>();

	// Texture related resources
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap2;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;


	// Terrain additions
	Entity* terrainEntity;
	Mesh* terrainMesh;
	SimplePixelShader* terrainPS;


	// Blend (or "splat") map
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> terrainBlendMapSRV;

	// Textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> terrainTexture0SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> terrainTexture1SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> terrainTexture2SRV;

	// Normal maps
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> terrainNormals0SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> terrainNormals1SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> terrainNormals2SRV;


};

