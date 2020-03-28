#include "ShaderIncludes.hlsli"


cbuffer ExternalData : register(b0)
{
	DirectionalLight dLight1;
	DirectionalLight dLight2;
	DirectionalLight dLight3;

	PointLight pLight1;

	float specInt;

	float3 cameraPosition;
}


// Texture-related resources
Texture2D diffuseTexture	: register(t0);
Texture2D normalMap			: register(t1);
SamplerState samplerOptions : register(s0);



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


float4 GetFinalColorDir(DirectionalLight directionalLight, float3 surfaceNormal, float3 surfaceColor)
{
	// Calculate the final pixel color
	float3 finalColor =
		Diffuse(surfaceNormal, directionalLight.Direction) * directionalLight.DiffuseColor * surfaceColor +
		directionalLight.AmbientColor * surfaceColor;

	return float4(finalColor, 1);
}


float4 GetFinalColorPoint(PointLight pointLight, float3 worldPos, float3 surfaceNormal, float3 surfaceColor)
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
float4 main(VertexToPixelNormalMap input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.uv).rgb;
	surfaceColor *= input.color.rgb;

	// grab the normal map sample and unpack the normal
	float3 normalFromMap = normalMap.Sample(samplerOptions, input.uv).rgb * 2 - 1;

	// Create the TBN matrix
	//  We need to transform the unpacked normal from tangent space into world space
	float3 N = input.normal; // must be normalized
	float3 T = input.tangent; // must be normalized
	T = normalize(T - N * dot(T, N)); // Gram-Schmidt orthogonalization
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	// Calculate the vector from the pixel's world position to the camera
	float3 toCamera = normalize(cameraPosition - input.worldPos);

	float4 directionalLightColor =
		GetFinalColorDir(dLight1, input.normal, surfaceColor) +
		SpecularPhong(input.normal, dLight1.Direction, toCamera, specInt * 64.0f); //+
		//GetFinalColorDir(dLight2, input.normal, input.color) +
		//GetFinalColorDir(dLight3, input.normal, input.color);

	float4 pointLightColor =
		GetFinalColorPoint(pLight1, input.worldPos, input.normal, surfaceColor) +
		SpecularPhong(input.normal, input.worldPos - pLight1.Position, toCamera, specInt * 64.0f);

	return directionalLightColor + pointLightColor;
}

