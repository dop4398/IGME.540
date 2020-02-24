#include "Game.h"
#include "Vertex.h"
#include <fstream>

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object
	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}

	delete vertexShader;
	delete pixelShader;
	delete camera;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Get the size as the next multiple of 16
	unsigned int size = vertexShader->GetBufferSize(0);
	size = (size + 15) / 16 * 16;

	// Describe the constant buffer
	CD3D11_BUFFER_DESC cbDesc = {}; // Sets to all zeros
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = size;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;

	// Create the camera
	//camera = new Camera(x, y, z, aspectRatio, mouseLookSpeed);
	camera = new Camera(0.0f, 0.0f, -5.0f, (float)(this->width / this->height), 2.0f);

	// Initialize lights
	DirectionalLight light1;
	light1.ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	light1.diffuseColor = XMFLOAT3(1, 1, 0);
	light1.direction = XMFLOAT3(1, -1, 0);
	lights.push_back(light1);
	DirectionalLight light2;
	light2.ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	light2.diffuseColor = XMFLOAT3(0, 1, 1);
	light2.direction = XMFLOAT3(-1, -1, 0);
	lights.push_back(light2);
	DirectionalLight light3;
	light3.ambientColor = XMFLOAT3(0.1f, 0.1f, 0.1f);
	light3.diffuseColor = XMFLOAT3(1, 0, 1);
	light3.direction = XMFLOAT3(1, 1, 0);
	lights.push_back(light3);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device.Get(), context.Get(),
		GetFullPathTo_Wide(L"VertexShader.cso").c_str());

	pixelShader = new SimplePixelShader(device.Get(), context.Get(),
		GetFullPathTo_Wide(L"PixelShader.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	XMFLOAT3 normal = XMFLOAT3(0, 0, -1);
	XMFLOAT2 uv = XMFLOAT2(0, 0);


	// mesh0 - triangle
	Vertex vertices0[] =
	{
		{ XMFLOAT3(+0.0f, +0.4f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.2f, -0.1f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.2f, -0.1f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.2f, -0.1f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.2f, -0.1f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.0f, -0.6f, +0.0f), normal, uv }
	};
	int indices0[] = { 0, 1, 2, 3, 4, 5 };
	// Add it to entities
	entities.push_back(new Entity(new Mesh(
		vertices0, 
		6, 
		indices0, 
		6, 
		device),
		new Material(pixelShader, vertexShader, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f))
	));
	// do it again to ensure sharing meshes works
	entities.push_back(new Entity(new Mesh(
		vertices0,
		6,
		indices0,
		6,
		device),
		new Material(pixelShader, vertexShader, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)) // material from our materials list
	));


	// mesh1 - rectangle
	Vertex vertices1[] =
	{
		{ XMFLOAT3(-0.9f, +0.9f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.4f, +0.9f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.4f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.4f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.9f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(-0.9f, +0.9f, +0.0f), normal, uv }
	};
	int indices1[] = { 0, 1, 2, 3, 4, 5 };
	entities.push_back(new Entity(new Mesh(
		vertices1,
		6,
		indices1,
		6,
		device),
		new Material(pixelShader, vertexShader, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f))
	));


	// mesh2 - triforce
	Vertex vertices2[] =
	{
		{ XMFLOAT3(+0.4f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.5f, +0.3f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.6f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.6f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.7f, +0.3f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.8f, +0.0f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.5f, +0.3f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.6f, +0.6f, +0.0f), normal, uv },
		{ XMFLOAT3(+0.7f, +0.3f, +0.0f), normal, uv }
	};
	int indices2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	entities.push_back(new Entity(
		new Mesh(
			vertices2,
			9,
			indices2,
			9,
			device),
		new Material(pixelShader, vertexShader, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f))
	));


	// mesh3 - cone
	entities.push_back(new Entity(
		new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device),
		new Material(pixelShader, vertexShader, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
	));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// update the camera
	if(camera != 0)
		camera->UpdateProjectionMatrix((float)(this->width / this->height));
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->GetTransform()->Rotate(0, 0, 1 * deltaTime);
		entities[i]->GetTransform()->CreateWorldMatrix();
	}

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// Update the camera
	camera->Update(deltaTime, this->hWnd);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// Set the vertex and pixel shaders to use for the next Draw() command ** done below in the for loop
	//  - These don't technically need to be set every frame
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw

	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	//context->IASetInputLayout(inputLayout.Get()); // Removed due to SimpleShader implementation


	pixelShader->SetData(
		"dLight1",
		&lights[0],
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"dLight2",
		&lights[1],
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"dLight3",
		&lights[2],
		sizeof(DirectionalLight));

	pixelShader->CopyAllBufferData();
	

	// Loop through all of the entities being drawn
	for (int i = 0; i < entities.size(); i++)
	{
		// Activate the current material's shaders
		entities[i]->GetMaterial()->GetVertexShader()->SetShader();
		entities[i]->GetMaterial()->GetPixelShader()->SetShader();
		// Collecting data locally
		SimpleVertexShader* vsData = entities[i]->GetMaterial()->GetVertexShader();
		vsData->SetFloat4("colorTint", entities[i]->GetMaterial()->GetColorTint());
		vsData->SetMatrix4x4("world", entities[i]->GetTransform()->GetWorldMatrix());
		vsData->SetMatrix4x4("view", camera->GetView());
		vsData->SetMatrix4x4("projection", camera->GetProjection());

		vsData->CopyAllBufferData();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		//  - for this demo, this step *could* simply be done once during Init(),
		//    but I'm doing it here because it's often done multiple times per frame
		//    in a larger application/game
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

	
		context->IASetVertexBuffers(0, 1, entities[i]->GetMesh()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(entities[i]->GetMesh()->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			entities[i]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}


	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}