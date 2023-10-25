#include "pch.h"
#include "Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/TransformComponent.h"

void Renderer::PassMain()
{
	auto actors = renderables[RenderableType::Opaque];
	if (actors.empty())
		return;

	for (const auto& actor : actors)
	{
		auto renderable = actor->GetComponent<MeshRendererComponent>();
		if (!renderable)
			continue;

		auto transform = actor->GetTransform_Raw();
		if (!transform)
			continue;

		D3D11_PipelineState pipeline_state;
		pipeline_state.input_layout       = renderable->GetInputLayout().get();
		pipeline_state.primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline_state.vertex_shader      = renderable->GetVertexShader().get();
		pipeline_state.pixel_shader = renderable->GetPixelShader().get();
		pipeline_state.rasterizer_state = rasterizers[RasterizerStateType::Cull_Back_Solid].get();

		if (pipeline->Begin(pipeline_state))
		{
			pipeline->SetVertexBuffer(renderable->GetVertexBuffer().get());
			pipeline->SetIndexBuffer(renderable->GetIndexBuffer().get());

			cpu_object_buffer.matWorld = transform->GetWorldMatrix();
			UpdateObjectBuffer();

			pipeline->SetConstantBuffer(0, ShaderScope_VS, gpu_camera_buffer.get());
			pipeline->SetConstantBuffer(1, ShaderScope_VS, gpu_object_buffer.get());

			pipeline->DrawIndexed
			(
				renderable->GetIndexBuffer()->GetCount(),
				renderable->GetIndexBuffer()->GetOffset(),
				renderable->GetVertexBuffer()->GetOffset()
			);
			pipeline->End();
		}


	}

}
