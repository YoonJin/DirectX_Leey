#pragma once

enum class Direction
{
	Up,
	Right,
	Down,
	Left
};

/*
	 Up
Left	Right
	Down
*/

class MyRect
{
public:
	MyRect(class Graphics* graphics, 
		const Color& color);
	~MyRect();

	Vec3& GetScale() { return scale; }
	Vec3& GetPosition() { return  position; }

	void SetPosition(const Vec3& position) 
	{
		this->position = position;
	}
	void SetScale(const Vec3& scale)
	{
		this->scale = scale;
	}
	void SetIntersectColor(const Color& color) 
	{
		this->instersect_color = color;
	}
	auto IsActive() { return this->is_active; }
	void SetActive(const bool& is_active) 
	{ this->is_active = is_active; }

	void Update();
	void Render(D3D11_Pipeline* pipeline);

	virtual void Event() = 0;
protected:
	virtual void Move() = 0;

protected:
	D3D11_VertexBuffer* vertexBuffer        = nullptr;
	D3D11_IndexBuffer* indexBuffer          = nullptr;
	D3D11_InputLayout* inputLayout          = nullptr;
	D3D11_Shader* vertex_shader             = nullptr;
	D3D11_Shader* pixel_shader              = nullptr;
	D3D11_ConstantBuffer* gpu_buffer        = nullptr;
	D3D11_RasterizerState* rasterizer_state = nullptr;

	D3D11_ConstantBuffer* color_buffer = nullptr;

	Vec3 position = Vec3(0, 0, 0);
	Vec3 scale    = Vec3(100, 100, 1);

	Matrix world;

	Color instersect_color = Color(1, 1, 1, 1);
	bool  is_active        = true;
	bool  isConflict       = false;
};