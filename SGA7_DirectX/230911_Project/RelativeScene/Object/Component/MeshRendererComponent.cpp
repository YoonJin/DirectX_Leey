#include "pch.h"
#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	:IComponent(context, actor, transform)
{
	graphics = context->GetSubsystem<Graphics>();
}

void MeshRendererComponent::Initialize()
{

}

void MeshRendererComponent::Update()
{
}

void MeshRendererComponent::Destroy()
{
}

void MeshRendererComponent::SetStandardMesh(MeshType type)
{
	D3D11_Geometry<D3D11_VertexTexture> geometry;

	if (type == MeshType::Quad)
		Geometry_Generator::CreateQuad(geometry);

	vertex_buffer = std::make_shared<D3D11_VertexBuffer>(graphics);
	vertex_buffer->Create(geometry.GetVertices());

	index_buffer = std::make_shared<D3D11_IndexBuffer>(graphics);
	index_buffer->Create(geometry.GetIndices());
}

void MeshRendererComponent::SetStandardMaterial(std::wstring path)
{
	vertex_shader = std::make_shared<D3D11_Shader>(graphics);
	vertex_shader->Create(ShaderScope_VS, path );

	pixel_shader = std::make_shared<D3D11_Shader>(graphics);
	pixel_shader->Create(ShaderScope_PS, path);

	input_layout = std::make_shared<D3D11_InputLayout>(graphics);
	input_layout->Create(D3D11_VertexTexture::descs, D3D11_VertexTexture::count, vertex_shader->GetShaderBlob());
}
