#pragma once

#include "Types.h"

// Window
#include <Windows.h>
#include <assert.h>
#include <string>


// STL
#include <iostream>
#include <sstream>
#include <vector>
#include <limits>
#include <map>
#include <unordered_map>
#include <random>
#include <chrono>

using namespace std;

// DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

using namespace DirectX;

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
	#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
	#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

#pragma warning(disable : 4506)

typedef unsigned int uint;

// Macro Function
#define SAFE_DELETE(p) { if (p) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) {delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr; } }

#define CHECK(p) assert(SUCCEEDED(p))

// Math
#include "Math/MyMath.h"

// Timer
#include "Timer/Timer.h"

// Intersect
#include "Intersect/Intersect.h"


// Utility
#include "Utility/Geometry_Generator.h"


// Framework
#include "Core/Settings.h"

// Main System
#include "Core/Context.h"

// Sub System
#include "Core/Subsystem/Graphics.h"
#include "Core/Subsystem/SceneManager.h"
#include "Core/Subsystem/Renderer/Renderer.h"


// Wrapper Class
#include "Core/D3D11/Vertex/D3D11_Vertex.h"
#include "Core/D3D11/Vertex/D3D11_Geometry.h"
#include "Core/D3D11/InputAssembler/D3D11_VertexBuffer.h"
#include "Core/D3D11/InputAssembler/D3D11_IndexBuffer.h"
#include "Core/D3D11/InputAssembler/D3D11_InputLayout.h"
#include "Core/D3D11/Shader/D3D11_Shader.h"
#include "Core/D3D11/Shader/D3D11_ConstantBuffer.h"
#include "Core/D3D11/Shader/D3D11_SamplerState.h"
#include "Core/D3D11/Shader/D3D11_Texture.h"
#include "Core/D3D11/Rasterizer/D3D11_RasterizerState.h"
#include "Core/D3D11/Rasterizer/D3D11_Viewport.h"
#include "Core/D3D11/OutputMerger/D3D11_BlendState.h"
#include "Core/D3D11/D3D11_Pipeline.h"
