// HLSL (High-Level Shading Language)

// ������ �ϴ� ��� ����
// 1. �������� ������ : ���α׷��� ���� �Ǳ� ���� �̸� ������ �Ͽ� ���̳ʸ� ���·� ������ ���´�.
//                     �� ����� ����ð��� �߰����� ������ ������尡 �����Ƿ�, 
//                     ������ ����ȭ �Ҽ� ������, 
//                     ������ ������ ���� �ܰ迡�� �̸� ���� �� �ִ�. 
//                     Fx �����Ϸ��� ���� ������ ����� �������ο��� ������ �� �� �ִ�.
// 2. ���� �ð� ������ : ���α׷� ���� �߿� �������� �ȴ�. �� ����� ������ �ϸ� ���̴���
//                      �������� �����ϰ� �ݿ��ϴ� ���� ���� ���ֹǷ�, ���߰� ����� ��������
//                      ������ �� �ִ�. 'D3DCompileFromFile'�� ���� �Լ��� ����Ͽ� ����
//                      �ð��� ������ �� �� �ִ�.


// ���� ���̴��� ���� �Է� ����ü ����
struct VS_INPUT
{
	float4 position : POSITION; // ������ 3D ��ġ
	//float4 color : COLOR;		
	float2 uv		: TEXCOORD; // �ؽ�ó ��ǥ
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION; // ȭ�� ���������� ���� ��ġ (�ý��� ��)
	float2 uv		: TEXCOORD; // �ؽ�ó ��ǥ
};

cbuffer CameraBuffer : register(b0)
{
	row_major matrix matView;
	row_major matrix matProjection;
}

// cbuffer == constantBuffer
cbuffer TransformBuffer : register(b1)
{
	// row_major : ����� ��Ұ� �޸𸮿� ����ɶ� 
	// �� �켱 ������ �����϶�.
	row_major matrix matWorld;
}

cbuffer TextureBuffer : register(b2)
{
	float4 offset;
}

// ���� ���̴� (VS) �Լ�
// �Է� ���� �����͸� �����ͼ� �׷��Ƚ� ������������ ���� �ܰ�� ���
VS_OUTPUT VS(VS_INPUT input)
{
	// ���� == World -> View -> Projection
	VS_OUTPUT output;
	output.position = mul(input.position, matWorld);
	output.position = mul(output.position, matView);
	output.position = mul(output.position, matProjection);

	output.uv = input.uv; // �ؽ�ó ��ǥ

	return output; // ��� ��ȯ
}

// ���̴����� �� �ؽ�ó�� ������ ��, �׷��Ƚ� ������������ �ؽ�ó �������� 0���� ����Ͽ� �ش� �ؽ�ó ���ҽ��� ����ȴ�.

// register(t0) : �׷��Ƚ� ���������� ���� �߻�ȭ�� �������͸� Ÿ������.
// gpu �� �ִ� directX�� �����͸� �����ϱ� ���� �����̴�.
Texture2D texture0 : register(t0);

// ���÷� ���¸� ��Ÿ����. : �ؽ�ó�� ��Ÿ�� �� ���Ǵ� �ȼ� ������ ��Ģ ����
SamplerState sampler0 : register(s0);

// �ȼ� ���̴� (PS)�Լ�
// ���� ���̴��� ����� �޾� ���� �ȼ� ������ ���
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = texture0.Sample(sampler0, input.uv);

	//color.rgb = 1.0 - color.rgb;

	return color;
}