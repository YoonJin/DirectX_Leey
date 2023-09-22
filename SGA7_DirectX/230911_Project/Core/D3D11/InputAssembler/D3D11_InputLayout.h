#pragma once

class D3D11_InputLayout final
{
public:
	D3D11_InputLayout(class Graphics* graphics);
	~D3D11_InputLayout();

	auto GetResource() const { return input_layout; }

	void Create(D3D11_INPUT_ELEMENT_DESC* descs, const uint& count, ID3DBlob* blob);
	void Clear();

private:
	ID3D11Device* device			=	nullptr;
	ID3D11InputLayout* input_layout =   nullptr;
};