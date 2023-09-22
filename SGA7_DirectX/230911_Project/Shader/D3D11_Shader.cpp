#include "pch.h"
#include "D3D11_Shader.h"

D3D11_Shader::D3D11_Shader(Graphics* graphics)
{
}

D3D11_Shader::~D3D11_Shader()
{
}

const char* D3D11_Shader::GetEntryPoint() const
{
	return nullptr;
}

const char* D3D11_Shader::GetTargetProfile() const
{
	return nullptr;
}

const char* D3D11_Shader::GetShaderModel() const
{
	return nullptr;
}

void D3D11_Shader::Create(const ShaderScope& scope, const std::string& shader)
{
}

void D3D11_Shader::Clear()
{
}


void* D3D11_Shader::Compile(const ShaderScope& scope, const std::string& shader)
{
	return nullptr;
}
