#include "pch.h"
#include "D3D11_Shader.h"

D3D11_Shader::D3D11_Shader(Graphics* graphics)
{
	device = graphics->GetDevice();
}

D3D11_Shader::~D3D11_Shader()
{
	Clear();
}

const char* D3D11_Shader::GetEntryPoint() const
{
	static const char* entry_point_empty = nullptr;
	static const char* entry_point_vs    = "VS";
	static const char* entry_point_ps    = "PS";

	switch (shader_scope)
	{
	case ShaderScope_VS: return entry_point_vs;
	case ShaderScope_PS: return entry_point_ps;
	}

	return entry_point_empty;
}

const char* D3D11_Shader::GetTargetProfile() const
{
	static const char* target_profile_empty = nullptr;

#if defined(GRAPHICS_API_VERSION_D3D11)
	static const char* target_profile_vs = "vs_5_0";
	static const char* target_profile_ps = "ps_5_0";
#elif defined(GRAPHICS_API_VERSION_D3D12)
	static const char* target_profile_vs = "vs_6_0";
	static const char* target_profile_ps = "vs_6_0";
#endif

	switch (shader_scope)
	{
	case ShaderScope_VS: return target_profile_vs;
	case ShaderScope_PS: return target_profile_ps;
	}

	return target_profile_empty;
}

const char* D3D11_Shader::GetShaderModel() const
{
#if defined(GRAPHICS_API_VERSION_D3D11)
	static const char* shader_model = "5_0";
#elif defined(GRAPHICS_API_VERSION_D3D12)
	static const char* shader_model = "6_0";
#endif

	return shader_model;
}

void D3D11_Shader::Create(const ShaderScope& scope, const std::wstring& shader)
{
	shader_scope = scope;

	compilation_state = CompilationState::Compiling;
	resource = Compile(scope, shader);
	compilation_state = HasResource() ? CompilationState::Succeeded : CompilationState::Failed;
}

void D3D11_Shader::Clear()
{
	switch (shader_scope)
	{
		case ShaderScope_VS:
		{
			auto shader = static_cast<ID3D11VertexShader*>(resource);
			SAFE_RELEASE(shader);
			break;
		}
		case ShaderScope_PS:
		{
			auto shader = static_cast<ID3D11PixelShader*>(resource);
			SAFE_RELEASE(shader);
			break;
		}
	}

	SAFE_RELEASE(shader_blob);

	name			  = "";
	path		      = "";
	shader_scope	  = ShaderScope_Unknown;
	compilation_state = CompilationState::Unknown;
}


void* D3D11_Shader::Compile(const ShaderScope& scope, const std::wstring& shader)
{
	ID3DBlob* error = nullptr;

	// * COMPILE_DEBUG : 이 플래그를 설정하면 쉐이더 디버깅 정보를 생성한다.
	// 디버그 정보를 포함하면 쉐이더 코드 내에서 디버거를 사용해 단계별로 실행하고
	// 변수 값을 검사하는 등 디버깅 작업을 수행할 수 있다.
	// * COMPILE_SKIP_OPTIMIZATION : 이 플래그를 사용하면, 쉐이더 컴파일 과정에서
	// 최적화를 건너뛴다. 최적화를 건너 뛰면 컴파일 속도가 빨라질 수 있으나
	// 실행성능은 최적화 되지 않은 상태로 유지된다.
	const uint compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		shader.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		GetEntryPoint(),
		GetTargetProfile(),
		compileFlag,
		0,
		&shader_blob,
		&error);


	if (error)
	{
		auto str = static_cast<const char*>(error->GetBufferPointer());
		MessageBoxA(nullptr, str, "Shader Error!!!", MB_OK);
	}

	CHECK(hr);

	void* shader_resource = nullptr;

	if (shader_blob)
	{
		switch (shader_scope)
		{
		case ShaderScope_VS:
			hr = device->CreateVertexShader
			(
				shader_blob->GetBufferPointer(),
				shader_blob->GetBufferSize(),
				nullptr,
				reinterpret_cast<ID3D11VertexShader**>(&shader_resource)
			);
			CHECK(hr);
			break;
		case ShaderScope_PS:
			hr = device->CreatePixelShader
			(
				shader_blob->GetBufferPointer(),
				shader_blob->GetBufferSize(),
				nullptr,
				reinterpret_cast<ID3D11PixelShader**>(&shader_resource)
			);
			CHECK(hr);
			break;
		}
	}

	return shader_resource;
}
