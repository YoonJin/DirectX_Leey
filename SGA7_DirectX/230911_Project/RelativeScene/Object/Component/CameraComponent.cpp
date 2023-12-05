#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

CameraComponent::CameraComponent(Context* const context, Actor* const actor, TransformComponent* const transform)
	:IComponent(context, actor, transform)
{
}

void CameraComponent::Initialize()
{
	view = Matrix::Identity;
	proj = Matrix::Identity;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Destroy()
{
}

void CameraComponent::UpdateConstantBuffer()
{
	if (!gpu_buffer)
	{
		gpu_buffer = std::make_shared<D3D11_ConstantBuffer>
			(context->GetSubsystem<Graphics>());
		gpu_buffer->Create<CameraBuffer>();
	}

	auto gpu_data = gpu_buffer->Map<CameraBuffer>();
	{
		CameraBuffer cpuData;
		cpuData.matView       = view;
		cpuData.matProjection = proj;

		::memcpy(gpu_data, &cpuData, sizeof(CameraBuffer));
	}
	gpu_buffer->Unmap();
}

void CameraComponent::UpdateViewMatrix()
{
	Vec3 position = transform->GetPosition();
	Vec3 forward  = transform->GetForward();
	Vec3 up       = transform->GetUp();

	view = XMMatrixLookAtLH(position, position + forward, up);
}

void CameraComponent::UpdateProjectionMatrix()
{
	proj = XMMatrixOrthographicLH(Settings::Get().GetWidth(),
		Settings::Get().GetHeight(), 0.f, 1.f);
}
