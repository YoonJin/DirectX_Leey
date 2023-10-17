#pragma once
#include "IComponent.h"

class CameraComponent final : public IComponent
{
	CameraComponent
	(
		class Actor* const actor,
		class TransformComponent* const transform
	);
	~CameraComponent() = default;

	void Initialize() override;
	void Update()     override;
	void Destroy()    override;

	const Matrix& GetViewMatrix() const
	{  return view;  }
	const Matrix& GetProjectionMatrix() const
	{  return proj;  }

	auto GetConstantBuffer() const { return gpu_buffer; }
	void UpdateConstantBuffer();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	Matrix view;
	Matrix proj;

	std::shared_ptr<class D3D11_ConstantBuffer> gpu_buffer;

};