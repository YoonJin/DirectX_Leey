#pragma once

#include "Types.h"

// Window
#include <Windows.h>
#include <assert.h>
#include <string>


// STL
#include <iostream>
#include <vector>

using namespace std;

// DirectX
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
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


typedef unsigned int uint;

// Macro Function
#define SAFE_DELETE(p) { if (p) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) {delete[] (p); (p) = nullptr; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr; } }

#define CHECK(p) assert(SUCCEEDED(p))



#include "Utility/Geometry_Generator.h"


// Framework
#include "Core/Settings.h"
#include "Core/Graphics.h"

#include "Core/D3D11/Vertex/D3D11_Vertex.h"
#include "Core/D3D11/Vertex/D3D11_Geometry.h"
#include "Core/D3D11/InputAssembler/D3D11_VertexBuffer.h"
#include "Core/D3D11/InputAssembler/D3D11_IndexBuffer.h"
#include "Core/D3D11/InputAssembler/D3D11_InputLayout.h"
#include "Core/D3D11/Shader/D3D11_Shader.h"
#include "Core/D3D11/Shader/D3D11_ConstantBuffer.h"
#include "Core/D3D11/Shader/D3D11_Texture.h"
#include "Core/D3D11/Rasterizer/D3D11_RasterizerState.h"
#include "Core/D3D11/Rasterizer/D3D11_Viewport.h"
