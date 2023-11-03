#pragma once
#include "IComponent.h"

enum class MeshType : uint
{
	Quad
};

class MeshRendererComponent final : public IComponent
{
public:
	MeshRendererComponent
	(
		class Context* const context,
		class Actor* const actor,
		class TransformComponent* const transform
	);
	~MeshRendererComponent() = default;

	void Initialize() override;
	void Update()     override;
	void Destroy()    override;

	/*
		Mesh
	*/
	auto GetVertexBuffer() const { return vertex_buffer; }
	auto GetIndexBuffer() const { return index_buffer;  }
	void SetStandardMesh(MeshType type);

	/*
	*	Material
	*/
	auto GetVertexShader() const { return vertex_shader; }
	auto GetPixelShader() const { return pixel_shader; }
	auto GetInputLayout() const { return input_layout; }
	void SetStandardMaterial(std::wstring path);

private:
	class Graphics* graphics = nullptr;

	MeshType mesh_type = MeshType::Quad;
	std::shared_ptr<D3D11_VertexBuffer> vertex_buffer;
	std::shared_ptr<D3D11_IndexBuffer> index_buffer;
	std::shared_ptr<D3D11_InputLayout> input_layout;
	std::shared_ptr<D3D11_Shader> vertex_shader;
	std::shared_ptr<D3D11_Shader> pixel_shader;
};