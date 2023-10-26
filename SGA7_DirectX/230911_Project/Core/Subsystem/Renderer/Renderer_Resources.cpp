#include "pch.h"
#include "Renderer.h"

void Renderer::CreateConstantBuffers()
{
	gpu_camera_buffer = std::make_shared<D3D11_ConstantBuffer>(graphics);
	gpu_camera_buffer->Create<CameraBuffer>();

	gpu_object_buffer = std::make_shared<D3D11_ConstantBuffer>(graphics);
	gpu_object_buffer->Create<TransformBuffer>();

	gpu_animation_buffer = std::make_shared<D3D11_ConstantBuffer>(graphics);
	gpu_animation_buffer->Create<AnimationBuffer>();
}

void Renderer::CreateRasterizerStates()
{
	rasterizers[RasterizerStateType::Cull_Front_Solid] =
		std::make_shared<D3D11_RasterizerState>(graphics);
	rasterizers[RasterizerStateType::Cull_Front_Solid]->Create(D3D11_CULL_FRONT, D3D11_FILL_SOLID);

	rasterizers[RasterizerStateType::Cull_Back_Solid] =
		std::make_shared<D3D11_RasterizerState>(graphics);
	rasterizers[RasterizerStateType::Cull_Back_Solid]->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);

	rasterizers[RasterizerStateType::Cull_None_Solid] =
		std::make_shared<D3D11_RasterizerState>(graphics);
	rasterizers[RasterizerStateType::Cull_None_Solid]->Create(D3D11_CULL_NONE, D3D11_FILL_SOLID);

	rasterizers[RasterizerStateType::Cull_Front_Wireframe] =
		std::make_shared<D3D11_RasterizerState>(graphics);
	rasterizers[RasterizerStateType::Cull_Front_Wireframe]->Create(D3D11_CULL_FRONT, D3D11_FILL_WIREFRAME);

	rasterizers[RasterizerStateType::Cull_Back_Wireframe] =
		std::make_shared<D3D11_RasterizerState>(graphics);
	rasterizers[RasterizerStateType::Cull_Back_Wireframe]->Create(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);

	rasterizers[RasterizerStateType::Cull_None_Wireframe] =
		std::make_shared<D3D11_RasterizerState>(graphics);
	rasterizers[RasterizerStateType::Cull_None_Wireframe]->Create
	(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);
}

void Renderer::CreateBlendStates()
{
	blend_states[BlendStateType::Disabled] = std::make_shared<D3D11_BlendState>(graphics);
	blend_states[BlendStateType::Disabled]->Create(false);

	blend_states[BlendStateType::Alpha] = std::make_shared<D3D11_BlendState>(graphics);
	blend_states[BlendStateType::Alpha]->Create(true);
}