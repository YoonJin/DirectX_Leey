#include "pch.h"
#include "Execute.h"

Execute::Execute()
{
	// 리소스들을 생성 초기화 해준다.
	graphics = new Graphics();
	graphics->CreateDeviceAndSwapChain();
	graphics->CreateRenderTargetView(
	static_cast<uint>(Settings::Get().GetWidth()),
		static_cast<uint>(Settings::Get().GetHeight()));
	graphics->SetViewport(
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

	// 카메라의 위치
	Vec3 eyePosition = { 0, 0, 0 };
	// 카메라가 바라보는 대상의 위치
	Vec3 lookAtPosition = { 0, 0, 1 };
	// 카메라의 수직방향을 나타내는 벡터
	Vec3 upVector = { 0, 1, 0 };

	_transformData.matWorld = Matrix::Identity;
	_transformData.matView = XMMatrixLookAtLH(eyePosition, lookAtPosition, upVector);
	_transformData.matProjection = XMMatrixOrthographicLH(Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0, 1);
}

Execute::~Execute()
{
	SAFE_RELEASE(_blendState);
	SAFE_RELEASE(_samplerState);
	
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
	//_transformData.offset.x += 0.003f;
	//_transformData.offset.y += 0.003f;

	Matrix matScale    = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);

	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matWorld = matScale * matRotation * matTranslation; // SRT
	_transformData.matWorld = matWorld;


	// 상수버퍼 세팅 부분. (건드리면 안됨.)
	{
		
		auto buffer = _gpuBuffer->Map<TransformData>();
		{
			// _transformData의 데이터를 매핑된 상수 버퍼로 복사한다.
			::memcpy(buffer, &_transformData, sizeof(_transformData));
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
		graphics->GetDeviceContext()->RSSetState(_rasterizerState);

		// PS
		graphics->GetDeviceContext()->PSSetShader(static_cast<ID3D11PixelShader*>(_pixelShader->GetResource()), nullptr, 0);
		graphics->GetDeviceContext()->PSSetShaderResources(0, 1, &_shaderResourceView);
		graphics->GetDeviceContext()->PSSetSamplers(0, 1, &_samplerState);

		// OM
		//graphics->GetDeviceContext()->Draw(_vertices.size(), 0);
		graphics->GetDeviceContext()->OMSetBlendState(_blendState, nullptr, 0xFFFFFFFF);
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
	// 래스터라이저 상태 설명 구조체
	D3D11_RASTERIZER_DESC desc;
	// 구조체 초기화
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.FillMode = D3D11_FILL_SOLID;   // 다각형 내부 픽셀을 채워주세요.
		desc.CullMode = D3D11_CULL_BACK;    // 오브젝트의 보이지 않는 뒷면은 
		                                    // 그리지 않겠다.
		desc.FrontCounterClockwise = false; // 정면은 시계방향으로 정의
	}
	HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &_rasterizerState);
	CHECK(hr);
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
	D3D11_SAMPLER_DESC desc; //샘플러 상태 설명 구조체
	ZeroMemory(&desc, sizeof(desc)); // 구조체 초기화
	{
		// 텍스처 좌표가 텍스처의 범위를 벗어났을 때 어떻게 처리할지 결정하는 방식
		// 텍스처 UV좌표같은 경우 0~1 범위 내에 있지만, 이것을 벗어난 경우
		// 지정된 경계색상으로 채워주겠다.
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER; // U좌표 경계처리
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;// V좌표 경계처리
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1; // 경계 색상 빨강
		desc.BorderColor[1] = 0; // 경계 색상 녹색
		desc.BorderColor[2] = 0; // 경계 색상 파랑
		desc.BorderColor[3] = 1; // 경계 색상 알파
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 선형 필터링 사용
		desc.MaxAnisotropy = 16; // 최대 이방성 필터링

		// MIP : 텍스처를 여러 해상도로 미리 계산하여 저장하는 기법
		desc.MaxLOD = FLT_MAX; // 최대 MIP 레벨
		desc.MinLOD = FLT_MIN; // 최소 MIP 레벨
		desc.MipLODBias = 0.0f; // MIP LOD 바이어스

		graphics->GetDevice()->CreateSamplerState(&desc, &_samplerState);
	}
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
	D3D11_BLEND_DESC desc; // 블렌딩 상태 설명 구조체
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC)); // 구조체 초기화
	{
		// 알파 투 커버리지를 비활성화 한다.
		// 이 옵션은 알파 값을 사용하여 멀티 샘플 앤티 앨리어싱의 
		// 커버리지를 결정한다.
		desc.AlphaToCoverageEnable = false;
		// 독립적인 렌더 타겟 블렌딩을 비활성화 한다.
		// false로 설정하면 모든 렌더 타겟은 첫 번째 렌더 타겟의 블렌드 설정을
		// 사용한다.
		desc.IndependentBlendEnable = false;



		// 첫 번째 렌더 타겟에 대한 블렌딩을 활성화 한다.
		desc.RenderTarget[0].BlendEnable = true;
		// 소스 블렌드 팩터를 소스 알파로 설정한다. 
		// 즉 소스 픽셀의 알파 값을 사용 한다. 
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		// 목적지 블렌드 팩터를 소스 알파의 역수로 설정한다.
		// 즉, (1 - 소스 알파 값)을 사용한다.
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		// 알파 채널에 대한 블렌드 연산을 덧셈으로 설정한다.
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		
		// 렌더 타겟 쓰기 마스크를 설정하여 RGBA 채널 모두를 쓰기를 허용한다.
		desc.RenderTarget[0].RenderTargetWriteMask = 
			D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	// 위에서 설정한 블렌드 상태 설명을 사용하여 블렌드 상태 객체를 생성한다.
	HRESULT hr = graphics->GetDevice()->CreateBlendState(&desc, &_blendState);
	CHECK(hr);
}

// DirectX Tex 라이브러리 이용해서 이미지 불러오는 함수
void Execute::CreateSRV()
{   
	// DirectX::TexMetadata md : 텍스처 메타데이터 구조체
	// 메타 데이터 : 텍스처의 차원, 형식, 크기등의 정보가 포함된다.
	DirectX::TexMetadata md;   

	// DirectX::ScratchImage img : 이미지 데이터를 저장할 객체.
	// 이미지의 픽셀 데이터를 담을 수 있음
	DirectX::ScratchImage img; // 스크래치 이미지 객체

	HRESULT hr = ::LoadFromWICFile(L"Assets/Skeleton.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	// 셰이더 리소스 뷰 생성
	// 셰이더 리소스 뷰 : GPU가 리소스(예: 텍스처, 버퍼 등)에 접근할 수 있는 인터페이스 제공
	hr = ::CreateShaderResourceView(graphics->GetDevice(), img.GetImages(),
		img.GetImageCount(), md, &_shaderResourceView);
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