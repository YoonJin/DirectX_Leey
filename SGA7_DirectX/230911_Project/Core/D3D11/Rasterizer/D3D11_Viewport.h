#pragma once

class D3D11_Viewport final
{
public:
	D3D11_Viewport
	(
		const float& x		= 0.0f,
		const float& y		= 0.0f,
		const float& width	= 0.0f,
		const float& height = 0.0f,
		const float& min_depth = 0.0f,
		const float& max_depth = 1.0f
	);
	D3D11_Viewport(const D3D11_Viewport& rhs);
	~D3D11_Viewport() = default;

	bool operator==(const D3D11_Viewport& rhs) const
	{ return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height && min_depth == rhs.min_depth && max_depth == rhs.max_depth; }
	bool operator!=(const D3D11_Viewport& rhs) const
	{ return !(*this == rhs); }

	bool IsDefined() const 
	{
		return x != 0.0f || y != 0.0f || width != 0.0f || height != 0.0f || min_depth != 0.0f || max_depth != 0.0f;
	}
	auto AspectRatio() const { return width / height; }

public:
	static const D3D11_Viewport Undefined_viewport;

public:
	float x			= 0.0f;
	float y			= 0.0f;
	float width		= 0.0f;
	float height	= 0.0f;
	float min_depth = 0.0f;
	float max_depth = 0.0f;
};