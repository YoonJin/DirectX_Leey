#pragma once

class D3D11_SamplerState final
{
public:
	D3D11_SamplerState(class Graphics* graphics);
	~D3D11_SamplerState();

	auto GetResource() const { return state; }

	void Create
	(
		const D3D11_FILTER& filter,
		const D3D11_TEXTURE_ADDRESS_MODE& address_mode
	);
	void Clear();

private:
	ID3D11Device* device	     = nullptr;
	ID3D11SamplerState* state    = nullptr;
};