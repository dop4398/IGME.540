
// Directional Light struct
struct DirectionalLight
{
	float3 AmbientColor;
	// padding done by compiler
	float3 DiffuseColor;
	// same
	float3 Direction;
};

cbuffer ExternalData : register(b0)
{
	DirectionalLight dLight1;
	DirectionalLight dLight2;
	DirectionalLight dLight3;
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
};

float4 GetFinalColor(DirectionalLight directionalLight, float3 surfaceNormal, float4 surfaceColor)
{
	// Calculate the normalized direction to the light
	float3 dirToLight = normalize(-directionalLight.Direction);

	// Calculate the light amount using the N dot L equation
	float3 lightAmount = dot(surfaceNormal, dirToLight);
	lightAmount = saturate(lightAmount); // clamp it between 0 and 1

	// Calculate the final pixel color
	float3 finalColor = lightAmount * directionalLight.DiffuseColor * surfaceColor +
		directionalLight.AmbientColor * surfaceColor;

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

	return GetFinalColor(dLight1, input.normal, input.color) +
		GetFinalColor(dLight2, input.normal, input.color) + 
		GetFinalColor(dLight3, input.normal, input.color);
}

