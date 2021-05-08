Texture2D textures: register(t0);
Texture2D textures2: register(t1);
Texture2D textures3: register(t2);
SamplerState colorSampler : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projMatrix;
	float4 valores;
};

cbuffer TiempoSkyDome : register(b1)
{
	int tiempo;
	int cambio;
	int a;
	float b;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);

	vsOut.tex0 = vertex.tex0;

	return vsOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET
{
	float4 finalColor1 = textures.Sample(colorSampler, pix.tex0);
	float4 finalColor2 = textures2.Sample(colorSampler, pix.tex0);
	float4 finalColor3 = textures3.Sample(colorSampler, pix.tex0);
	float4 text;
	
	if (tiempo == 0) {
		text = finalColor1;
		if (a == 1) {
			text = lerp(finalColor1, finalColor2, b);
		}
		

	}
	if (tiempo == 1) {
		text = finalColor2;
		if (a == 1) {
			text = lerp(finalColor2, finalColor3, b);
		}

	}
	if (tiempo == 2) {
		text = finalColor3;
		if (a == 1) {
			text = lerp(finalColor3, finalColor1, b);
		}

	}
	
	return text;
	
}