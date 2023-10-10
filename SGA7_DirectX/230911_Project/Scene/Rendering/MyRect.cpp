#include "pch.h"
#include "MyRect.h"

MyRect::MyRect(Graphics* graphics, const Color& color)
{
	D3D11_Geometry<D3D11_VertexColor> geometry;
	Geometry_Generator::CreateQuad(geometry, color);

	vertexBuffer = new D3D11_VertexBuffer(graphics);
	vertexBuffer->Create(geometry.GetVertices());

	indexBuffer = new D3D11_IndexBuffer(graphics);
	indexBuffer->Create(geometry.GetIndices());

	vertex_shader = new D3D11_Shader(graphics);
	vertex_shader->Create(ShaderScope_VS, L"Assets/Color.hlsl");

	pixel_shader = new D3D11_Shader(graphics);
	pixel_shader->Create(ShaderScope_PS, L"Assets/Color.hlsl");

	inputLayout = new D3D11_InputLayout(graphics);
	inputLayout->Create(D3D11_VertexColor::descs, D3D11_VertexColor::count, 
		vertex_shader->GetShaderBlob());

	gpu_buffer = new D3D11_ConstantBuffer(graphics);
	gpu_buffer->Create<TransformBuffer>();

	rasterizer_state = new D3D11_RasterizerState(graphics);
	rasterizer_state->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);

	color_buffer = new D3D11_ConstantBuffer(graphics);
	color_buffer->Create<ColorBuffer>();

	// Create World
	{
		world = Matrix::Identity;
	
		// World
		Matrix S = XMMatrixScaling(scale.x, scale.y, scale.z);
		Matrix T = XMMatrixTranslation(position.x, position.y, position.z);

		world = S * T;
	}
}

MyRect::~MyRect()
{
	SAFE_DELETE(color_buffer);
	SAFE_DELETE(rasterizer_state);
	SAFE_DELETE(gpu_buffer);
	SAFE_DELETE(inputLayout);
	SAFE_DELETE(pixel_shader);
	SAFE_DELETE(vertex_shader);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(vertexBuffer);
}

void MyRect::Update()
{
	Move();

	Matrix S = XMMatrixScaling(scale.x, scale.y, scale.z);
	Matrix T = XMMatrixTranslation(position.x, position.y, position.z);

	world = S * T;

	// -> World 행렬 변환 상수버퍼 적용 코드
	{
		auto buffer = gpu_buffer->Map<TransformBuffer>();
		{
			TransformBuffer cpuData;
			cpuData.matWorld = world;

			::memcpy(buffer, &cpuData, sizeof(TransformBuffer));
		}
		gpu_buffer->Unmap();
	}
	// 컬러값 상수 변수 적용 코드
	{
		auto buffer = color_buffer->Map<ColorBuffer>();
		{
			ColorBuffer cpuData;
			cpuData.color      = instersect_color;
			cpuData.isConflict = isConflict;

			::memcpy(buffer, &cpuData, sizeof(ColorBuffer));
		}
		color_buffer->Unmap();
	}
}

void MyRect::Render(D3D11_Pipeline* pipeline)
{
	if (is_active)
	{
		D3D11_PipelineState pipeline_state;
		pipeline_state.input_layout = inputLayout;
		pipeline_state.primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline_state.vertex_shader = vertex_shader;
		pipeline_state.pixel_shader = pixel_shader;
		pipeline_state.rasterizer_state = rasterizer_state;

		if (pipeline->Begin(pipeline_state))
		{
			pipeline->SetVertexBuffer(vertexBuffer);
			pipeline->SetIndexBuffer(indexBuffer);
			pipeline->SetConstantBuffer(1, ShaderScope_VS, gpu_buffer);
			pipeline->SetConstantBuffer(2, ShaderScope_PS,
				color_buffer);
			pipeline->DrawIndexed(indexBuffer->GetCount(), indexBuffer->GetOffset(), vertexBuffer->GetOffset());

			pipeline->End();
		}
	}
}
