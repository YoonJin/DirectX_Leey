struct VertexInput
{
	float4 position : POSITION0;
	float2 uv       : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
};

cbuffer CameraBuffer : register(b0)
{
	row_major matrix view;
	row_major matrix proj;
}

cbuffer TransformBuffer : register(b1)
{
	row_major matrix world;
}

cbuffer AnimationBuffer : register(b2)
{
	float2 sprite_offset;
	float2 sprite_size;
	float2 texture_size;
	float is_animated;
}

PixelInput VS(VertexInput input)
{
	PixelInput output;
	output.position = float4(input.position.xy * sprite_size, 0.0f, 1.0f);
	output.position = mul(output.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

	output.uv = input.uv;
	output.uv *= sprite_size / texture_size;
	output.uv += sprite_offset / texture_size;

	return output;
}

Texture2D source_texture : register(t0);
SamplerState samp        : register(s0);

float4 PS(PixelInput input) : SV_Target
{

	if (is_animated == 1.0f)
		return source_texture.Sample(samp, input.uv);
	else
		return float4(1, 0, 0, 1);
}