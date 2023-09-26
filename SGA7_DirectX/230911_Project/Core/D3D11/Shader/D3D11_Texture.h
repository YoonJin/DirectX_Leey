#pragma once

class D3D11_Texture final
{
public:
	D3D11_Texture(class Graphics* graphics);
	~D3D11_Texture();

	auto GetResource() const { return shader_resource; }

	void Create(const std::wstring& path);
	void Clear();

private:
	ID3D11Device* device = nullptr;
	ID3D11ShaderResourceView* shader_resource = nullptr;
};