#pragma once

class Camera final
{
public:
	Camera();
	~Camera() = default;

	const Matrix& GetViewMatrix() const { return view; }
	const Matrix& GetProjectionMatrix() const { return proj; }

	void Update();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	Matrix view;
	Matrix proj;
};