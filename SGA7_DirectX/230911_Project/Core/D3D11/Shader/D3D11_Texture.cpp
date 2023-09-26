#include "pch.h"
#include "D3D11_Texture.h"

D3D11_Texture::D3D11_Texture(Graphics* graphics)
{
	device = graphics->GetDevice();
}

D3D11_Texture::~D3D11_Texture()
{
	Clear();
}

void D3D11_Texture::Create(const std::wstring& path)
{
	TexMetadata  md;
	ScratchImage img;

	const wchar_t* wPath = path.c_str();
	HRESULT hr = ::LoadFromWICFile(wPath, WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(device, img.GetImages(), img.GetImageCount(), md,
		&shader_resource);

	CHECK(hr);
}

void D3D11_Texture::Clear()
{
	SAFE_RELEASE(shader_resource);
}
