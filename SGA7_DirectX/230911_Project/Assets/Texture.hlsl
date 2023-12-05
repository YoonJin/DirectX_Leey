// HLSL (High-Level Shading Language)

// 컴파일 하는 방식 종류
// 1. 오프라인 컴파일 : 프로그램이 실행 되기 전에 미리 컴파일 하여 바이너리 형태로 저장해 놓는다.
//                     이 방식은 실행시간에 추가적인 컴파일 오버헤드가 없으므로, 
//                     성능을 최적화 할수 있으며, 
//                     컴파일 에러는 빌드 단계에서 미리 잡을 수 있다. 
//                     Fx 컴파일러와 같은 도구를 사용해 오프라인에서 컴파일 할 수 있다.
// 2. 실행 시간 컴파일 : 프로그램 실행 중에 컴파일이 된다. 이 방식은 개발을 하며 쉐이더를
//                      동적으로 수정하고 반영하는 것을 쉽게 해주므로, 개발과 디버깅 과정에서
//                      유용할 수 있다. 'D3DCompileFromFile'과 같은 함수를 사용하여 실행
//                      시간에 컴파일 할 수 있다.


// 정점 셰이더에 대한 입력 구조체 정의
struct VS_INPUT
{
	float4 position : POSITION; // 정점의 3D 위치
	//float4 color : COLOR;		
	float2 uv		: TEXCOORD; // 텍스처 좌표
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION; // 화면 공간에서의 정점 위치 (시스템 값)
	float2 uv		: TEXCOORD; // 텍스처 좌표
};

cbuffer CameraBuffer : register(b0)
{
	row_major matrix matView;
	row_major matrix matProjection;
}

// cbuffer == constantBuffer
cbuffer TransformBuffer : register(b1)
{
	// row_major : 행렬의 요소가 메모리에 저장될때 
	// 행 우선 순서로 저장하라.
	row_major matrix matWorld;
}

cbuffer TextureBuffer : register(b2)
{
	float4 offset;
}

// 정점 셰이더 (VS) 함수
// 입력 정점 데이터를 가져와서 그래픽스 파이프라인의 다음 단계로 출력
VS_OUTPUT VS(VS_INPUT input)
{
	// 순서 == World -> View -> Projection
	VS_OUTPUT output;
	output.position = mul(input.position, matWorld);
	output.position = mul(output.position, matView);
	output.position = mul(output.position, matProjection);

	output.uv = input.uv; // 텍스처 좌표

	return output; // 결과 반환
}

// 셰이더에서 이 텍스처를 참조할 때, 그래픽스 파이프라인이 텍스처 레지스터 0번을 사용하여 해당 텍스처 리소스와 연결된다.

// register(t0) : 그래픽스 파이프라인 내의 추상화된 레지스터를 타나낸다.
// gpu 상에 있는 directX가 데이터를 저장하기 위한 공간이다.
Texture2D texture0 : register(t0);

// 샘플러 상태를 나타낸다. : 텍스처를 나타낼 때 사용되는 픽셀 계산등의 규칙 정의
SamplerState sampler0 : register(s0);

// 픽셀 셰이더 (PS)함수
// 정점 셰이더의 출력을 받아 최종 픽셀 색상을 계산
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = texture0.Sample(sampler0, input.uv);

	//color.rgb = 1.0 - color.rgb;

	return color;
}