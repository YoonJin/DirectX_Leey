#pragma once

#pragma region Types.h
// DirectX에서 Vector 및 Matrix 등 수학관련 자료형을 쓰기 위한 데이터 정의 파일
#pragma endregion

#include <Windows.h>
#include <DirectXMath.h>
#include "MyLibraries/SimpleMath.h"

using Color = DirectX::XMFLOAT4;

using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;