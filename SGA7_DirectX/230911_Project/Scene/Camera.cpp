#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	view = Matrix::Identity;
	proj = Matrix::Identity;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	Vec3 lookAtPosition = Vec3(0, 0, 1);
	Vec3 upDirection    = Vec3(0, 1, 0);

	// 1. position       : 카메라의 위치를 나타내는 벡터 값
	// 2. lookAtPosition : 카메라가 바라보는 지점을 나타내는 벡터값
	// 3. upDirection    : 카메라의 위쪽 방향을 나타내는 벡터
	view = ::XMMatrixLookAtLH(this->position, 
		this->position + lookAtPosition, upDirection);
}

void Camera::UpdateProjectionMatrix()
{
	// Orthographics
	// Perspective
	proj = XMMatrixOrthographicLH(Settings::Get().GetWidth(), 
		Settings::Get().GetHeight(), 0.0f, 1.0f);
}
