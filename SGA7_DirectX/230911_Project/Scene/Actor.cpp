#include "pch.h"
#include "Actor.h"
#include "Component/TransformComponent.h"
#include "Component/MeshRendererComponent.h"

Actor::Actor(Context* const context)
	 : context(context)
{
	AddComponent<TransformComponent>();
}

Actor::~Actor()
{
	components.clear();
}

void Actor::Initialize()
{
	for (const auto& component : components)
		component->Initialize();
}

void Actor::Update()
{
	if (!is_active)
		return;

	for (const auto& component : components)
	{
		if (!component->IsEnabled())
			continue;

		component->Update();
	}
}

void Actor::Render(D3D11_Pipeline* const pipeline)
{
	if (!is_active)
		return;

	if (auto mesh_renderer = GetComponent<MeshRendererComponent>())
	{
		D3D11_PipelineState pipeline_state;
		pipeline_state.input_layout = mesh_renderer->GetInputLayout().get();
		pipeline_state.primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline_state.vertex_shader      = mesh_renderer->GetVertexShader().get();
		pipeline_state.pixel_shader       = mesh_renderer->GetPixelShader().get();

		if (pipeline->Begin(pipeline_state))
		{
			pipeline->SetVertexBuffer(mesh_renderer->GetVertexBuffer().get());
			pipeline->SetIndexBuffer(mesh_renderer->GetIndexBuffer().get());

			transform->UpdateConstantBuffer();

			pipeline->SetConstantBuffer(1, ShaderScope_VS, transform->GetConstantBuffer().get());

			pipeline->DrawIndexed
			(
				mesh_renderer->GetIndexBuffer()->GetCount(),
				mesh_renderer->GetIndexBuffer()->GetOffset(),
				mesh_renderer->GetVertexBuffer()->GetOffset()
			);
			
			pipeline->End();
		}
	}
}

void Actor::Destroy()
{
	for (const auto& component : components)
		component->Destroy();
}

auto Actor::AddComponent(const ComponentType& type) -> const std::shared_ptr<IComponent>
{
	// TODO : 
	return std::shared_ptr<IComponent>();
}

bool Actor::HasComponent(const ComponentType& type)
{
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
			return true;
	}
	return false;
}
