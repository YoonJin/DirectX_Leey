#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Camera.h"

Execute::Execute()
{
	// 리소스들을 생성 초기화 해준다.
	graphics = new Graphics();
	graphics->CreateDeviceAndSwapChain();
	graphics->CreateRenderTargetView(
	static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight()));


	CreateGeometry();    // 1.
	CreateVS();          // 2.
	CreateInputLayout(); // 3.
	CreatePS();          // 4.

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	// 이미지 텍스처 로드
	CreateSRV();
	CreateConstantBuffer();

	camera = new Camera();
	{
		world = Matrix::Identity;

		// World
		Matrix S = XMMatrixScaling(250, 250, 1);
		Matrix R = XMMatrixRotationX(0) * XMMatrixRotationY(0) * XMMatrixRotationZ(0);
		Matrix T = XMMatrixTranslation(0, 0, 0);
	
		world = S * R * T;
	}
}

Execute::~Execute()
{
	SAFE_DELETE(camera);
	SAFE_DELETE(_blendState);
	SAFE_DELETE(_samplerState);
	SAFE_DELETE(_texture);
	SAFE_DELETE(_rasterizerState);
	SAFE_DELETE(_gpuBuffer);
	SAFE_DELETE(_inputLayout);
	SAFE_DELETE(_pixelShader);
	SAFE_DELETE(_vertexShader);
	SAFE_DELETE(_indexBuffer);
	SAFE_DELETE(_vertexBuffer);
	SAFE_DELETE(graphics);
}

void Execute::Update()
{
	camera->Update();

	// 상수버퍼 세팅 부분. (건드리면 안됨.)
	{
		auto buffer = _gpuBuffer->Map<TransformData>();
		{
			TransformData cpuData;
			cpuData.matWorld	  = world;
			cpuData.matView		  = camera->GetViewMatrix();
			cpuData.matProjection = camera->GetProjectionMatrix();
			// _transformData의 데이터를 매핑된 상수 버퍼로 복사한다.
			::memcpy(buffer, &cpuData, sizeof(TransformData));
		}
		_gpuBuffer->Unmap();
	
	}
}

void Execute::Render()
{
	graphics->RenderBegin();
	// 렌더링 파이프라인에 따른 여러가지 처리들을 아래에 기재
	{
		// IA
		ID3D11Buffer* buffers[] = { _vertexBuffer->GetResource() };
		graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, buffers, &_vertexBuffer->GetStride(), &_vertexBuffer->GetOffset());

		graphics->GetDeviceContext()->IASetIndexBuffer(_indexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, _indexBuffer->GetOffset());

		// Input Layout -> 데이터의 구조를 정의했다!
		graphics->GetDeviceContext()->IASetInputLayout(_inputLayout->GetResource());



		// IASetPrimitiveTopology : 삼각형으로 그리겠다.
		graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		graphics->GetDeviceContext()->VSSetShader(static_cast<ID3D11VertexShader*>(_vertexShader->GetResource()), nullptr, 0);


		ID3D11Buffer* cbuffers[] = { _gpuBuffer->GetResource() };
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, 1, cbuffers);

		// RS
		graphics->GetDeviceContext()->RSSetState(_rasterizerState->GetResource());

		// PS
		graphics->GetDeviceContext()->PSSetShader(static_cast<ID3D11PixelShader*>(_pixelShader->GetResource()), nullptr, 0);

		// Resource View
		ID3D11ShaderResourceView* shader_resources[] = { _texture->GetResource() };
		graphics->GetDeviceContext()->PSSetShaderResources(0, 1, shader_resources);

		ID3D11SamplerState* samplers[] = { _samplerState->GetResource() };
		graphics->GetDeviceContext()->PSSetSamplers(0, 1, samplers);

		// OM
		float factor         = _blendState->GetBlendFactor();
		float blend_factor[] = { factor, factor, factor, factor };
		graphics->GetDeviceContext()->OMSetBlendState(_blendState->GetResource(), blend_factor, _blendState->GetSampleMask());

		graphics->GetDeviceContext()->DrawIndexed(geometry.GetIndexCount(), 0, 0);
	}
	graphics->RenderEnd();
}

void Execute::CreateGeometry()
{
	// 시계방향으로 삼각형을 그린다.
	// 시계방향으로 그려야만 카메라를 기준으로한 앞면으로 그려진다.
	/*
		  |      1
		  |
	--------------
		  |
	0	  |      2
	*/

	Geometry_Generator::CreateQuad(geometry);

	// GPU 데이터 세팅
	// VertexBuffer
	_vertexBuffer = new D3D11_VertexBuffer(graphics);
	_vertexBuffer->Create(geometry.GetVertices());

	_indexBuffer = new D3D11_IndexBuffer(graphics);
	_indexBuffer->Create(geometry.GetIndices());
}

void Execute::CreateInputLayout()
{
	_inputLayout = new D3D11_InputLayout(graphics);
	_inputLayout->Create(D3D11_VertexTexture::descs, D3D11_VertexTexture::count,
		_vertexShader->GetShaderBlob());
}

void Execute::CreateVS()
{
	_vertexShader = new D3D11_Shader(graphics);
	_vertexShader->Create(ShaderScope_VS, L"Assets/Texture.hlsl");
}

void Execute::CreatePS()
{
	_pixelShader = new D3D11_Shader(graphics);
	_pixelShader->Create(ShaderScope_PS, L"Assets/Texture.hlsl");
}

// 3D 객체를 2D화면에 어떻게 렌더링 할지를 결정하는 규칙을 정의한다.
void Execute::CreateRasterizerState()
{
	_rasterizerState = new D3D11_RasterizerState(graphics);
	_rasterizerState->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);
}

// 텍스처를 샘플링 할때 어떻게 픽셀들을 필터링하고 해석할지 결정하는 규칙의 집합.
// 샘플링 : 텍스처의 특정 위치에서 색상 또는 값을 가져오는 프로세스를 의미한다.
//          스크린의 각 픽셀에 어떤 색상을 그릴지를 결정하는데 사용된다.
//         -> 텍스처 좌표 : 3D 그래픽스에서 객체의 각 버텍스는 UV 좌표를 갖는다.
//                         이 좌표는 2D 텍스처 이미지 상의 위치를 가리킨다.
//         -> 샘플링 과정 : 텍스처 샘플링은 UV 좌표를 이용하여 텍스처에서 특정 색상 값을
//                         추출하는 것.
//         -> 필터링 과정 : UV좌표가 텍스처의 정확한 픽셀 위치를 가리키지 않을때,
//                         예를 들면 텍스처가 확대되거나 축소될 때 발생한다.
//                         이럴 때는 여러 텍스처 픽셀의 색상 값을 고려하여 결과 색상값을 결정해야한다.
//                         이러한 연산을 '필터링' 이라고 한다.
void Execute::CreateSamplerState()
{
	_samplerState = new D3D11_SamplerState(graphics);
	_samplerState->Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
}

// 픽셀의 색상을 어떻게 혼합할지 정의하는 상태이다.
// 두개 이상의 색상 값을 조합하여 최종 색상을 계산하는 과정이다.
// 특히 블렌딩은 투명한 물체를 렌더링 하거나 다양한 특수 효과를 만드는데 사용된다.
// 블랜드 스테이트는 다음과 같은 정보를 포함할 수 있다.
/*
   1. Source Blend Factor : 소스 색상(즉, 현재 렌더링 중인 픽셀의 색상)에 곱해질 계수이다.
   2. Destination Blend Factor : 목적지 색상(즉, 프레임 버퍼에 이미 존재하는 색상)에 곱해질
								 계수
   3. Blend Operation : 소스와 목적지 색상이 어떻게 결합 될지를 나타내는 연산이다.
						예를 들면, 덧셈, 뺄셈 등.
   4. Alpha Blending : 알파 값을 사용한 블렌딩을 지정한다. 알파 값은 투명도를 나타내며,
					   알파 블렌딩을 사용하면 물체의 투명한 부분을 표현할 수 있다.

					   예를 들어, 소스 알파를 사용하여 목적지 색상의 반전된 소스 알파와 결합
					   하는 경우, 투명한 물체 뒤의 내용을 부분적으로 볼 수 있으며,
					   이를 통해 유리나 물과 같은 물체를 표현할 수도 있다.
*/
void Execute::CreateBlendState()
{
	_blendState = new D3D11_BlendState(graphics);
	_blendState->Create(true);
}

// DirectX Tex 라이브러리 이용해서 이미지 불러오는 함수
void Execute::CreateSRV()
{   
	_texture = new D3D11_Texture(graphics);
	_texture->Create(L"Assets/Skeleton.png");
}

// 상수 버퍼를 세팅하는데 사용한다.
// * 데이터 유형 : 상수 데이터를 저장한다. 이러한 데이터는 셰이더에서 사용되지만,
//                 셰이더 코드내에서 직접 변경할 수 없다.
// * 변경 빈도   : 프레임마다 변경이 될 수 있으며, 뷰변환, 프로젝션 변환, 라이트 속성,
//                 물체의 월드 변환 등 여러가지 상황에 따라 동적으로 업데이트 될 수 있다.
//
// -> 버텍스 버퍼의 많은 정점 데이터들을 셰이더 코드에 일일이 전달 하는 것 보다는,
// 상수 버퍼의 적은 데이터를 프레임마다 변경하며 셰이더에 전달하는 것이 훨씬 비용이 적게 든다.
void Execute::CreateConstantBuffer()
{
	_gpuBuffer = new D3D11_ConstantBuffer(graphics);
	_gpuBuffer->Create<TransformData>();
}