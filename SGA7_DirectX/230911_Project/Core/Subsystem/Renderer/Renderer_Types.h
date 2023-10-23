#pragma once
#include "pch.h"

enum class RenderableType : uint
{
	Opaque,
	Camera
};

enum class RasterizerStateType : uint
{
	Cull_Front_Solid,
	Cull_Back_Solid,
	Cull_None_Solid,
	Cull_Front_Wireframe,
	Cull_Back_Wireframe,
	Cull_None_Wireframe
};

enum class BlendStateType : uint
{
	Disabled,
	Alpha
};