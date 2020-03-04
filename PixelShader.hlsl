
// Directional Light struct
struct DirectionalLight
{
	float3 AmbientColor;
	// padding done by compiler
	float3 DiffuseColor;
	// same
	float3 Direction;
};


// Directional Light struct
struct PointLight
{
	float3 AmbientColor;
	// padding done by compiler
	float3 DiffuseColor;
	// same
	float3 Position;
};


cbuffer ExternalData : register(b0)
{
	DirectionalLight dLight1;
	DirectionalLight dLight2;
	DirectionalLight dLight3;

	PointLight pLight1;

	float specInt;

	float3 cameraPosition;
}


// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
};


// Calculates the diffuse given the surface normal and the direction of the light
float Diffuse(float3 normal, float3 lightDir)
{
	// we need the direction to the light so we get the negative normal of the light's direction
	return saturate(dot(normal, -normalize(lightDir)));
}


float SpecularPhong(float3 normal, float3 lightDir, float3 toCamera, float specExp)
{
	// reflection vector
	float3 refl = reflect(normalize(lightDir), normal);

	// 1. Get the dot product between the perfect reflection of the light
	//    and the vector to the camera, as that tells us how "close" we 
	//    are to seeing the perfect reflection.  
	// 2. Saturate to ensure it doesn't go negative on the back of the object
	// 3. Raise to a power to create a very quick "falloff" (so the highlight
	//    appears smaller if the object is shinier)
	return pow(saturate(dot(refl, toCamera)), specExp);
}


float4 GetFinalColorDir(DirectionalLight directionalLight, float3 surfaceNormal, float4 surfaceColor)
{
	// Calculate the final pixel color
	float3 finalColor = 
		Diffuse(surfaceNormal, directionalLight.Direction) * directionalLight.DiffuseColor * surfaceColor +
		directionalLight.AmbientColor * surfaceColor;

	return float4(finalColor, 1);
}


float4 GetFinalColorPoint(PointLight pointLight, float3 worldPos, float3 surfaceNormal, float4 surfaceColor)
{
	// Calculate the final pixel color
	float3 finalColor =
		Diffuse(surfaceNormal, worldPos - pointLight.Position) * pointLight.DiffuseColor * surfaceColor +
		pointLight.AmbientColor * surfaceColor;

	return float4(finalColor, 1);
}


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	// Calculate the vector from the pixel's world position to the camera
	float3 toCamera = normalize(cameraPosition - input.worldPos);

	float4 directionalLightColor =
		GetFinalColorDir(dLight1, input.normal, input.color) + 
		SpecularPhong(input.normal, dLight1.Direction, toCamera, specInt * 64.0f); //+
		//GetFinalColorDir(dLight2, input.normal, input.color) +
		//GetFinalColorDir(dLight3, input.normal, input.color);

	float4 pointLightColor =
		GetFinalColorPoint(pLight1, input.worldPos, input.normal, input.color) +
		SpecularPhong(input.normal, input.worldPos - pLight1.Position, toCamera, specInt * 64.0f);

	return directionalLightColor + pointLightColor;
	//return pointLightColor;
}

