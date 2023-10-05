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
	Vec3 position       = Vec3(0, 0, 0);
	Vec3 lookAtPosition = Vec3(0, 0, 1);
	Vec3 upDirection    = Vec3(0, 1, 0);
	view = ::XMMatrixLookAtLH(position, lookAtPosition, upDirection);
}

void Camera::UpdateProjectionMatrix()
{
	// Orthographics
	// Perspective
	proj = XMMatrixOrthographicLH(Settings::Get().GetWidth(), 
		Settings::Get().GetHeight(), 0.0f, 1.0f);
}
