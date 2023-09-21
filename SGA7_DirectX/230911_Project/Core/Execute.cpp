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
	SAFE_DELETE(graphics);
	SAFE_RELEASE(_blendState);
	SAFE_RELEASE(_samplerState);
	SAFE_RELEASE(_psBlob);
	SAFE_RELEASE(_pixelShader);
	SAFE_RELEASE(_vsBlob);
	SAFE_RELEASE(_vertexShader);
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_inputLayout);
	SAFE_RELEASE(_vertexBuffer);
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
		// ���ε� ���� ���ҽ� ����ü�� �����Ѵ�.
	// ���� : ��ǻ�� ���п��� �Ϲ������� �� ������ �Ǵ� ���ҽ��� �ٸ� ������ �Ǵ� ���ҽ���
	// �����ϰų� ������Ű�� ������ �ǹ�. ex) CPU->GPU���� �޸� ������ ���� �� ����
	// �Ʒ��� subResrouce ��ü�� �̿��ؼ� GPU �� ����� �����Ϳ� CPU�� ���� �����ϰ�
	// ������ �� �ְ� �����.

		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		// Map�� ����ϸ� GPU ���ҽ��� CPU�ҽ��� ������ �� �ִ�.
		// ���ε� ���¿����� CPU�� �ش� ���ҽ��� �����͸� �а� �� �� �ִ�.
		// �ش� ���°� �Ǹ� GPU�� �����Ϳ� ������ �Ұ����ϴ�.
		graphics->GetDeviceContext()->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			// _transformData�� �����͸� ���ε� ��� ���۷� �����Ѵ�.
			::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
		}
		// ��� ������ ������ �����Ͽ� GPU�� �ٽ� �����Ϳ� ���� �� �� �ֵ��� �Ѵ�.
		graphics->GetDeviceContext()->Unmap(_constantBuffer, 0);
	}
}

void Execute::Render()
{
	// * stride : �� ���� �������� ũ�⸦ ����Ʈ ������ ��Ÿ����.
	// * offset : ���� ���ۿ��� ù��° ���� �����ͱ����� ����Ʈ �������� ��Ÿ����.
	uint stride = sizeof(VertexTexture);
	uint offset = 0;

	graphics->RenderBegin();
	// ������ ���������ο� ���� �������� ó������ �Ʒ��� ����
	{
		// IA
		graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
		graphics->GetDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		// Input Layout -> �������� ������ �����ߴ�!
		graphics->GetDeviceContext()->IASetInputLayout(_inputLayout);
		// IASetPrimitiveTopology : �ﰢ������ �׸��ڴ�.
		graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// VS
		graphics->GetDeviceContext()->VSSetShader(_vertexShader, nullptr, 0);
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, 1, &_constantBuffer);

		// RS
		graphics->GetDeviceContext()->RSSetState(_rasterizerState);

		// PS
		graphics->GetDeviceContext()->PSSetShader(_pixelShader, nullptr, 0);
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

	geometry.AddVertex(VertexTexture(Vec3(-0.5f, -0.5f, +0.f), Vec2(0.f, 1.f)));
	geometry.AddVertex(VertexTexture(Vec3(-0.5f, +0.5f, 0.0f), Vec2(0.0f, 0.0f)));
	geometry.AddVertex(VertexTexture(Vec3(+0.5f, -0.5f, 0.0f), Vec2(1.0f, 1.0f)));
	geometry.AddVertex(VertexTexture(Vec3(+0.5f, +0.5f, 0.0f), Vec2(1.0f, 0.0f)));
	geometry.AddIndex(0); geometry.AddIndex(1); geometry.AddIndex(2); 
	geometry.AddIndex(2); geometry.AddIndex(1); geometry.AddIndex(3);


	// GPU ������ ����
	// VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		{
			// GPU�� �����͸� ���� ���� �ְ�, CPU�� ���� �Ұ�
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			// �ش� ���۴� �������� �����ϴ� ���۷� Ȱ���ϰڴ�.
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			// ���� ������
			desc.ByteWidth = geometry.GetVertexByteWidth();
		}

		// cpu�� �����͸� gpu�� �Ѱ��ֱ� ���� ����ü
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		{
			data.pSysMem = geometry.GetVertexPointer();
		}

		auto hr = graphics->GetDevice()->CreateBuffer(&desc, &data, &_vertexBuffer);
		CHECK(hr);
	}

	// IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.ByteWidth = geometry.GetIndexByteWidth();
		}

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = geometry.GetIndexPointer();

		HRESULT hr = graphics->GetDevice()->CreateBuffer(&desc, &data, &_indexBuffer);
		CHECK(hr);
	}
}

void Execute::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC); 

	// ������ ���� �ۼ�
	graphics->GetDevice()->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), &_inputLayout);
}

void Execute::CreateVS()
{
	LoadShaderFromFile(L"Assets/Texture.hlsl", "VS", "vs_5_0", &_vsBlob);
	HRESULT hr = graphics->GetDevice()->CreateVertexShader(_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(), nullptr, &_vertexShader);
}

void Execute::CreatePS()
{
	LoadShaderFromFile(L"Assets/Texture.hlsl", "PS", "ps_5_0", &_psBlob);
	HRESULT hr = graphics->GetDevice()->CreatePixelShader(_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(), nullptr, &_pixelShader);
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
	D3D11_BUFFER_DESC desc; // ���� ���� ����ü ����
	ZeroMemory(&desc, sizeof(desc));

	{
		desc.Usage = D3D11_USAGE_DYNAMIC; // 1. CPU���� �������� ������ ����
		                                  // 2. GPU���� �������� �бⰡ ����

		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
		desc.ByteWidth = sizeof(TransformData);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU���� �� ���ۿ� ���� ����
		                                              // ���� ������ �����
	}

	HRESULT hr = graphics->GetDevice()->CreateBuffer(&desc, nullptr, &_constantBuffer);
}

void Execute::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ID3DBlob** blob)
{
	// * COMPILE_DEBUG : �� �÷��׸� �����ϸ� ���̴� ����� ������ �����Ѵ�.
	// ����� ������ �����ϸ� ���̴� �ڵ� ������ ����Ÿ� ����� �ܰ躰�� �����ϰ�
	// ���� ���� �˻��ϴ� �� ����� �۾��� ������ �� �ִ�.
	// * COMPILE_SKIP_OPTIMIZATION : �� �÷��׸� ����ϸ�, ���̴� ������ ��������
	// ����ȭ�� �ǳʶڴ�. ����ȭ�� �ǳ� �ٸ� ������ �ӵ��� ������ �� ������
	// ���༺���� ����ȭ ���� ���� ���·� �����ȴ�.
	const uint compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob,
		nullptr);

	CHECK(hr);
}
