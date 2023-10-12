#pragma once

class Camera final
{
public:
	Camera();
	~Camera() = default;

	const Matrix& GetViewMatrix() const { return view; }
	const Matrix& GetProjectionMatrix() const { return proj; }

	void Update();

	void SetPosition(const Vec3 position) { this->position = position; }

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	Vec3 position;

	Matrix view;
	Matrix proj;
};