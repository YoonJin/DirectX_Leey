#include "pch.h"
#include "Execute.h"
#include "Graphics.h"
#include "Scene/Camera.h"

Execute::Execute()
{
	// ���ҽ����� ���� �ʱ�ȭ ���ش�.
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

	// �̹��� �ؽ�ó �ε�
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

	// ������� ���� �κ�. (�ǵ帮�� �ȵ�.)
	{
		auto buffer = _gpuBuffer->Map<TransformData>();
		{
			TransformData cpuData;
			cpuData.matWorld	  = world;
			cpuData.matView		  = camera->GetViewMatrix();
			cpuData.matProjection = camera->GetProjectionMatrix();
			// _transformData�� �����͸� ���ε� ��� ���۷� �����Ѵ�.
			::memcpy(buffer, &cpuData, sizeof(TransformData));
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
	_rasterizerState = new D3D11_RasterizerState(graphics);
	_rasterizerState->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);
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
	_samplerState = new D3D11_SamplerState(graphics);
	_samplerState->Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
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
	_blendState = new D3D11_BlendState(graphics);
	_blendState->Create(true);
}

// DirectX Tex ���̺귯�� �̿��ؼ� �̹��� �ҷ����� �Լ�
void Execute::CreateSRV()
{   
	_texture = new D3D11_Texture(graphics);
	_texture->Create(L"Assets/Skeleton.png");
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