#include "pch.h"
#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	:IComponent(context, actor, transform)
{
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
	D3D11_Geometry<D3D11_VertexColor> geometry;
	Geometry_Generator::CreateQuad(geometry, Color(1.f, 0.f, 1.f, 1.f));

	vertex_buffer = std::make_shared<D3D11_VertexBuffer>(context->GetSubsystem<Graphics>());
	vertex_buffer->Create(geometry.GetVertices());

	index_buffer = std::make_shared<D3D11_IndexBuffer>(context->GetSubsystem<Graphics>());
	index_buffer->Create(geometry.GetIndices());
}

void MeshRendererComponent::SetStandardMaterial()
{
	vertex_shader = std::make_shared<D3D11_Shader>(context->GetSubsystem<Graphics>());
	vertex_shader->Create(ShaderScope_VS, L"Assets/Color.hlsl");

	pixel_shader = std::make_shared<D3D11_Shader>(context->GetSubsystem<Graphics>());
	pixel_shader->Create(ShaderScope_PS, L"Assets/Color.hlsl");

	input_layout = std::make_shared<D3D11_InputLayout>(context->GetSubsystem<Graphics>());
	input_layout->Create(D3D11_VertexColor::descs, D3D11_VertexColor::count, vertex_shader->GetShaderBlob());
}
