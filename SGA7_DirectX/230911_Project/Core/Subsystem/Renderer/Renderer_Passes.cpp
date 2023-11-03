#include "pch.h"
#include "Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/MeshRendererComponent.h"
#include "Scene/Component/TransformComponent.h"
#include "Scene/Component/AnimatorComponent.h"
#include "Scene/Component/MoveScriptComponent.h"
#include "Scene/Component/TextureComponent.h"

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
		pipeline_state.pixel_shader       = renderable->GetPixelShader().get();
		pipeline_state.rasterizer_state   = rasterizers[RasterizerStateType::Cull_Back_Solid].get();
		pipeline_state.blend_state        = blend_states[BlendStateType::Alpha].get();


		if (pipeline->Begin(pipeline_state))
		{
			pipeline->SetVertexBuffer(renderable->GetVertexBuffer().get());
			pipeline->SetIndexBuffer(renderable->GetIndexBuffer().get());

			cpu_object_buffer.matWorld = transform->GetWorldMatrix();
			UpdateObjectBuffer();

			if (auto texture = actor->GetComponent<TextureComponent>())
			{
				cpu_texture_buffer.offset = Vec2(0, 0);
				UpdateTextureBuffer();

				const D3D11_Texture* data = texture->GetCurrentTexture()->GetTexture();
				pipeline->SetConstantBuffer(2, ShaderScope_VS | ShaderScope_PS,
					gpu_texture_buffer.get());
				pipeline->SetShaderResource(0, ShaderScope_PS, data);
			}
			else if (auto animator = actor->GetComponent<AnimatorComponent>())
			{
				auto moveScript = actor->GetComponent<MoveScriptComponent>();

				auto current_keyframe = animator->GetCurrentKeyframe(moveScript->GetCurrentDirection());
				cpu_animation_buffer.sprite_offset = current_keyframe->offset;
				cpu_animation_buffer.sprite_size   = current_keyframe->size;
				cpu_animation_buffer.texture_size  = animator->GetCurrentAnimation()->GetSpriteTextureSize();
				cpu_animation_buffer.is_animated   = 1.0f;
				UpdateAnimationBuffer();

				pipeline->SetConstantBuffer(2, ShaderScope_VS | ShaderScope_PS, gpu_animation_buffer.get());
				pipeline->SetShaderResource(0, ShaderScope_PS, animator->GetCurrentAnimation()->GetSpriteTexture().get());
			}
			
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
