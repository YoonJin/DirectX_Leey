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
	float4 color : COLOR;		// 정점의 색상
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION; // 화면 공간에서의 정점 위치 (시스템 값)
	float4 color    : COLOR;
};

// 정점 셰이더 (VS) 함수
// 입력 정점 데이터를 가져와서 그래픽스 파이프라인의 다음 단계로 출력
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output; // 출력 구조체를 생성
	output.position = input.position; // 입력 위치를 그대로 출력 위치로 설정
	output.color = input.color; // 입력 색상을 그대로 출력 색상으로 설정

	return output;
}

// 픽셀 셰이더 (PS)함수
// 정점 셰이더의 출력을 받아 최종 픽셀 색상을 계산
float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.color;
}