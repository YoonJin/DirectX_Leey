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
	float4 color : COLOR;		// ������ ����
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION; // ȭ�� ���������� ���� ��ġ (�ý��� ��)
	float4 color    : COLOR;
};


cbuffer CameraBuffer : register(b0)
{
	row_major matrix matView;
	row_major matrix matProjection;
}

cbuffer TransformBuffer : register(b1)
{
	row_major matrix matWorld;
}

cbuffer ColorBuffer : register(b2)
{
	float4 color;
	// 4byte;
	bool  isConflict;
}


// ���� ���̴� (VS) �Լ�
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output; // ��� ����ü�� ����
	output.position = mul(input.position, matWorld);
	output.position = mul(output.position, matView);
	output.position = mul(output.position, matProjection);

	output.color = input.color;

	return output; // ����� ��ȯ
}

// �ȼ� ���̴� (PS)�Լ�
// ���� ���̴��� ����� �޾� ���� �ȼ� ������ ���
float4 PS(VS_OUTPUT input) : SV_Target
{
	if (isConflict)
		return color;
	else
		return input.color;
}