#include "pch.h"
#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	:IComponent(context, actor, transform)
{
	graphics = context->GetSubsystem<Graphics>();
}

void MeshRendererComponent::Initialize()
{
	SetStandardMesh();
	SetStandardMaterial();
}

void MeshRendererComponent::Update()
{
}

void MeshRendererComponent::Destroy()
{
}

void MeshRendererComponent::SetStandardMesh()
{
	D3D11_Geometry<D3D11_VertexTexture> geometry;
	Geometry_Generator::CreateQuad(geometry);

	vertex_buffer = std::make_shared<D3D11_VertexBuffer>(graphics);
	vertex_buffer->Create(geometry.GetVertices());

	index_buffer = std::make_shared<D3D11_IndexBuffer>(graphics);
	index_buffer->Create(geometry.GetIndices());
}

void MeshRendererComponent::SetStandardMaterial()
{
	vertex_shader = std::make_shared<D3D11_Shader>(graphics);
	vertex_shader->Create(ShaderScope_VS, L"Assets/Animation.hlsl");

	pixel_shader = std::make_shared<D3D11_Shader>(graphics);
	pixel_shader->Create(ShaderScope_PS, L"Assets/Animation.hlsl");

	input_layout = std::make_shared<D3D11_InputLayout>(graphics);
	input_layout->Create(D3D11_VertexTexture::descs, D3D11_VertexTexture::count, vertex_shader->GetShaderBlob());
}
