
Texture2D texDiffuse : register(t0);


cbuffer lightcamerabuffer : register(b0)
{
	float4 lightPos;
	float4 camPos;
};



cbuffer phongbuffer : register(b1)
{
	
	float4 ka;
	float4 kd;
	float4 ks;

};

struct PSIn
{
	float4 posW : POSW;
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
};
 



//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float4 PS_main(PSIn input) : SV_Target
{
	// Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	// The 4:th component is opacity and should be = 1

	float4 L = normalize(lightPos - input.posW);
	float4 N = float4(input.Normal, 1);
	float4 V = normalize(camPos - input.posW);
	float4 R = reflect(-L, N);



	
	// Debug shading #2: map and return texture coordinates as a color (blue = 0)
	//return float4(input.TexCoord, 0, 1);
	//return float4(input.Normal * 0.5 + 0.5, 1);
	return float4 (ka * 0.5 + max(kd * dot(L, N), 0) + ks * pow(max(dot(R, V), 40), 0.0));
}