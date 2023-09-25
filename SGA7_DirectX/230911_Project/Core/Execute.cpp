#include "pch.h"
#include "Execute.h"

Execute::Execute()
{
	// ���ҽ����� ���� �ʱ�ȭ ���ش�.
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

	// �̹��� �ؽ�ó �ε�
	CreateSRV();
	CreateConstantBuffer();

	// ī�޶��� ��ġ
	Vec3 eyePosition = { 0, 0, 0 };
	// ī�޶� �ٶ󺸴� ����� ��ġ
	Vec3 lookAtPosition = { 0, 0, 1 };
	// ī�޶��� ���������� ��Ÿ���� ����
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


	// ������� ���� �κ�. (�ǵ帮�� �ȵ�.)
	{
		
		auto buffer = _gpuBuffer->Map<TransformData>();
		{
			// _transformData�� �����͸� ���ε� ��� ���۷� �����Ѵ�.
			::memcpy(buffer, &_transformData, sizeof(_transformData));
		}
		_gpuBuffer->Unmap();
	
	}
}

void Execute::Render()
{
	graphics->RenderBegin();
	// ������ ���������ο� ���� �������� ó������ �Ʒ��� ����
	{
		// IA
		ID3D11Buffer* buffers[] = { _vertexBuffer->GetResource() };
		graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, buffers, &_vertexBuffer->GetStride(), &_vertexBuffer->GetOffset());

		graphics->GetDeviceContext()->IASetIndexBuffer(_indexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, _indexBuffer->GetOffset());
		
		// Input Layout -> �������� ������ �����ߴ�!
		graphics->GetDeviceContext()->IASetInputLayout(_inputLayout->GetResource());



		// IASetPrimitiveTopology : �ﰢ������ �׸��ڴ�.
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
	// �ð�������� �ﰢ���� �׸���.
	// �ð�������� �׷��߸� ī�޶� ���������� �ո����� �׷�����.
	/*
		  |      1
		  |
	--------------
		  |
	0	  |      2
	*/

	Geometry_Generator::CreateQuad(geometry);

	// GPU ������ ����
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

// 3D ��ü�� 2Dȭ�鿡 ��� ������ ������ �����ϴ� ��Ģ�� �����Ѵ�.
void Execute::CreateRasterizerState()
{
	// �����Ͷ����� ���� ���� ����ü
	D3D11_RASTERIZER_DESC desc;
	// ����ü �ʱ�ȭ
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.FillMode = D3D11_FILL_SOLID;   // �ٰ��� ���� �ȼ��� ä���ּ���.
		desc.CullMode = D3D11_CULL_BACK;    // ������Ʈ�� ������ �ʴ� �޸��� 
		                                    // �׸��� �ʰڴ�.
		desc.FrontCounterClockwise = false; // ������ �ð�������� ����
	}
	HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &_rasterizerState);
	CHECK(hr);
}

// �ؽ�ó�� ���ø� �Ҷ� ��� �ȼ����� ���͸��ϰ� �ؼ����� �����ϴ� ��Ģ�� ����.
// ���ø� : �ؽ�ó�� Ư�� ��ġ���� ���� �Ǵ� ���� �������� ���μ����� �ǹ��Ѵ�.
//          ��ũ���� �� �ȼ��� � ������ �׸����� �����ϴµ� ���ȴ�.
//         -> �ؽ�ó ��ǥ : 3D �׷��Ƚ����� ��ü�� �� ���ؽ��� UV ��ǥ�� ���´�.
//                         �� ��ǥ�� 2D �ؽ�ó �̹��� ���� ��ġ�� ����Ų��.
//         -> ���ø� ���� : �ؽ�ó ���ø��� UV ��ǥ�� �̿��Ͽ� �ؽ�ó���� Ư�� ���� ����
//                         �����ϴ� ��.
//         -> ���͸� ���� : UV��ǥ�� �ؽ�ó�� ��Ȯ�� �ȼ� ��ġ�� ����Ű�� ������,
//                         ���� ��� �ؽ�ó�� Ȯ��ǰų� ��ҵ� �� �߻��Ѵ�.
//                         �̷� ���� ���� �ؽ�ó �ȼ��� ���� ���� ����Ͽ� ��� ������ �����ؾ��Ѵ�.
//                         �̷��� ������ '���͸�' �̶�� �Ѵ�.
void Execute::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc; //���÷� ���� ���� ����ü
	ZeroMemory(&desc, sizeof(desc)); // ����ü �ʱ�ȭ
	{
		// �ؽ�ó ��ǥ�� �ؽ�ó�� ������ ����� �� ��� ó������ �����ϴ� ���
		// �ؽ�ó UV��ǥ���� ��� 0~1 ���� ���� ������, �̰��� ��� ���
		// ������ ���������� ä���ְڴ�.
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER; // U��ǥ ���ó��
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;// V��ǥ ���ó��
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1; // ��� ���� ����
		desc.BorderColor[1] = 0; // ��� ���� ���
		desc.BorderColor[2] = 0; // ��� ���� �Ķ�
		desc.BorderColor[3] = 1; // ��� ���� ����
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // ���� ���͸� ���
		desc.MaxAnisotropy = 16; // �ִ� �̹漺 ���͸�

		// MIP : �ؽ�ó�� ���� �ػ󵵷� �̸� ����Ͽ� �����ϴ� ���
		desc.MaxLOD = FLT_MAX; // �ִ� MIP ����
		desc.MinLOD = FLT_MIN; // �ּ� MIP ����
		desc.MipLODBias = 0.0f; // MIP LOD ���̾

		graphics->GetDevice()->CreateSamplerState(&desc, &_samplerState);
	}
}

// �ȼ��� ������ ��� ȥ������ �����ϴ� �����̴�.
// �ΰ� �̻��� ���� ���� �����Ͽ� ���� ������ ����ϴ� �����̴�.
// Ư�� ������ ������ ��ü�� ������ �ϰų� �پ��� Ư�� ȿ���� ����µ� ���ȴ�.
// ���� ������Ʈ�� ������ ���� ������ ������ �� �ִ�.
/*
   1. Source Blend Factor : �ҽ� ����(��, ���� ������ ���� �ȼ��� ����)�� ������ ����̴�.
   2. Destination Blend Factor : ������ ����(��, ������ ���ۿ� �̹� �����ϴ� ����)�� ������
								 ���
   3. Blend Operation : �ҽ��� ������ ������ ��� ���� ������ ��Ÿ���� �����̴�.
						���� ���, ����, ���� ��.
   4. Alpha Blending : ���� ���� ����� ������ �����Ѵ�. ���� ���� ������ ��Ÿ����,
					   ���� ������ ����ϸ� ��ü�� ������ �κ��� ǥ���� �� �ִ�.

					   ���� ���, �ҽ� ���ĸ� ����Ͽ� ������ ������ ������ �ҽ� ���Ŀ� ����
					   �ϴ� ���, ������ ��ü ���� ������ �κ������� �� �� ������,
					   �̸� ���� ������ ���� ���� ��ü�� ǥ���� ���� �ִ�.
*/
void Execute::CreateBlendState()
{
	D3D11_BLEND_DESC desc; // ���� ���� ���� ����ü
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC)); // ����ü �ʱ�ȭ
	{
		// ���� �� Ŀ�������� ��Ȱ��ȭ �Ѵ�.
		// �� �ɼ��� ���� ���� ����Ͽ� ��Ƽ ���� ��Ƽ �ٸ������ 
		// Ŀ�������� �����Ѵ�.
		desc.AlphaToCoverageEnable = false;
		// �������� ���� Ÿ�� ������ ��Ȱ��ȭ �Ѵ�.
		// false�� �����ϸ� ��� ���� Ÿ���� ù ��° ���� Ÿ���� ���� ������
		// ����Ѵ�.
		desc.IndependentBlendEnable = false;



		// ù ��° ���� Ÿ�ٿ� ���� ������ Ȱ��ȭ �Ѵ�.
		desc.RenderTarget[0].BlendEnable = true;
		// �ҽ� ���� ���͸� �ҽ� ���ķ� �����Ѵ�. 
		// �� �ҽ� �ȼ��� ���� ���� ��� �Ѵ�. 
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		// ������ ���� ���͸� �ҽ� ������ ������ �����Ѵ�.
		// ��, (1 - �ҽ� ���� ��)�� ����Ѵ�.
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		// ���� ä�ο� ���� ���� ������ �������� �����Ѵ�.
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		
		// ���� Ÿ�� ���� ����ũ�� �����Ͽ� RGBA ä�� ��θ� ���⸦ ����Ѵ�.
		desc.RenderTarget[0].RenderTargetWriteMask = 
			D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	// ������ ������ ���� ���� ������ ����Ͽ� ���� ���� ��ü�� �����Ѵ�.
	HRESULT hr = graphics->GetDevice()->CreateBlendState(&desc, &_blendState);
	CHECK(hr);
}

// DirectX Tex ���̺귯�� �̿��ؼ� �̹��� �ҷ����� �Լ�
void Execute::CreateSRV()
{   
	// DirectX::TexMetadata md : �ؽ�ó ��Ÿ������ ����ü
	// ��Ÿ ������ : �ؽ�ó�� ����, ����, ũ����� ������ ���Եȴ�.
	DirectX::TexMetadata md;   

	// DirectX::ScratchImage img : �̹��� �����͸� ������ ��ü.
	// �̹����� �ȼ� �����͸� ���� �� ����
	DirectX::ScratchImage img; // ��ũ��ġ �̹��� ��ü

	HRESULT hr = ::LoadFromWICFile(L"Assets/Skeleton.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	// ���̴� ���ҽ� �� ����
	// ���̴� ���ҽ� �� : GPU�� ���ҽ�(��: �ؽ�ó, ���� ��)�� ������ �� �ִ� �������̽� ����
	hr = ::CreateShaderResourceView(graphics->GetDevice(), img.GetImages(),
		img.GetImageCount(), md, &_shaderResourceView);
}

// ��� ���۸� �����ϴµ� ����Ѵ�.
// * ������ ���� : ��� �����͸� �����Ѵ�. �̷��� �����ʹ� ���̴����� ��������,
//                 ���̴� �ڵ峻���� ���� ������ �� ����.
// * ���� ��   : �����Ӹ��� ������ �� �� ������, �亯ȯ, �������� ��ȯ, ����Ʈ �Ӽ�,
//                 ��ü�� ���� ��ȯ �� �������� ��Ȳ�� ���� �������� ������Ʈ �� �� �ִ�.
//
// -> ���ؽ� ������ ���� ���� �����͵��� ���̴� �ڵ忡 ������ ���� �ϴ� �� ���ٴ�,
// ��� ������ ���� �����͸� �����Ӹ��� �����ϸ� ���̴��� �����ϴ� ���� �ξ� ����� ���� ���.
void Execute::CreateConstantBuffer()
{
	_gpuBuffer = new D3D11_ConstantBuffer(graphics);
	_gpuBuffer->Create<TransformData>();
}