#pragma once

struct VertexColor final
{
	VertexColor()
		:position(0, 0, 0),
		 color(0, 0, 0, 0)
	{}

	VertexColor(const Vec3& position, const Color& color)
		:position(position)
		, color(color)
	{}

	Vec3 position;
	Color color;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};

struct VertexTexture final
{
	VertexTexture()
		: position(0, 0, 0)
		, uv(0, 0)
	{}

	VertexTexture(const Vec3& position, const Vec2& uv)
		: position(position)
		, uv(uv)
	{}

	Vec3 position;
	Vec2 uv;

	static D3D11_INPUT_ELEMENT_DESC descs[];
	static const uint count = 2;
};