#pragma once


enum ShaderScope : uint
{
	ShaderScope_Unknown,
	ShaderScope_VS,	       
	ShaderScope_PS          
};


enum class CompilationState : uint
{
	Unknown,
	Compiling,
	Succeeded,
	Filed
};

class D3D11_Shader final
{
public:
	D3D11_Shader(class Graphics* graphics);
	~D3D11_Shader();

	auto GetResource() const { return resource;  }
	bool HasResource() const { return resource != nullptr; }
	auto GetShaderBlob() const { return shader_blob; }

	auto GetName()	const { return name; }
	void SetName(const std::string& name) { this->name = name; }

	auto GetPath() const { return path; }
	auto GetShaderScope() const { return shader_scope;  }
	auto GetCompilationState() const { return compilation_state; }

	const char* GetEntryPoint() const;
	const char* GetTargetProfile() const;
	const char* GetShaderModel() const;

	bool IsCompiled() const { return compilation_state == CompilationState::Succeeded; }

	void Create(const ShaderScope& scope, const std::string& shader);
	void Clear();

private:
	void* Compile(const ShaderScope& scope, const std::string& shader);

private:
	ID3D11Device* device = nullptr;
	ID3DBlob* shader_blob = nullptr;
	void* resource;

	std::string name = "";
	std::string path = "";
	ShaderScope shader_scope = ShaderScope_Unknown;
	CompilationState compilation_state = CompilationState::Unknown;
};
