#include "pch.h"
#include "Renderer.h"
#include "RelativeScene/Scene/BaseScene.h"
#include "RelativeScene/Object/Actor.h"
#include "RelativeScene/Object/Component/CameraComponent.h"
#include "RelativeScene/Object//Component/MeshRendererComponent.h"

Renderer::Renderer(Context* const context) : IObserver(context)
{

}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	graphics = context->GetSubsystem<Graphics>();
	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));
	graphics->CreateDepthStencil(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));
	
	pipeline = std::make_shared<D3D11_Pipeline>(graphics);

	CreateConstantBuffers();
	CreateRasterizerStates();
	CreateBlendStates();

	return true;
}

void Renderer::Update()
{
	if (!camera)
		return;

	cpu_camera_buffer.matView = camera->GetViewMatrix();
	cpu_camera_buffer.matProjection = camera->GetProjectionMatrix();
	UpdateCameraBuffer();

	graphics->RenderBegin();
	PassMain();
	graphics->RenderEnd();
}

void Renderer::ReceivedNotify()
{

}

void Renderer::UpdateRenderables(BaseScene* const scene)
{
	auto actors = scene->GetActors();
	if (actors.empty())
		return;

	for (const auto& actor : actors)
	{
		auto camera_component = actor->GetComponent<CameraComponent>();
		auto mesh_renderer_component = actor->GetComponent<MeshRendererComponent>();

		if (camera_component)
		{
			renderables[RenderableType::Camera].push_back(actor.get());
			camera = camera_component.get();
		}

		if (mesh_renderer_component)
		{
			renderables[RenderableType::Opaque].push_back(actor.get());
		}
	}
}

void Renderer::UpdateCameraBuffer()
{
	auto buffer = gpu_camera_buffer->Map<CameraBuffer>();
	*buffer = cpu_camera_buffer;
	gpu_camera_buffer->Unmap();
}

void Renderer::UpdateObjectBuffer()
{
	auto buffer = gpu_object_buffer->Map<TransformBuffer>();
	*buffer = cpu_object_buffer;
	gpu_object_buffer->Unmap();
}

void Renderer::UpdateAnimationBuffer()
{
	auto buffer = gpu_animation_buffer->Map<AnimationBuffer>();
	*buffer = cpu_animation_buffer;
	gpu_animation_buffer->Unmap();
}

void Renderer::UpdateTextureBuffer()
{
	auto buffer = gpu_texture_buffer->Map<TextureBuffer>();
	*buffer = cpu_texture_buffer;
	gpu_texture_buffer->Unmap();
}


