#include "pch.h"
#include "D3D11_InputLayout.h"

D3D11_InputLayout::D3D11_InputLayout(Graphics* graphics)
{
	device = graphics->GetDevice();
}

D3D11_InputLayout::~D3D11_InputLayout()
{
	Clear();
}

void D3D11_InputLayout::Create(D3D11_INPUT_ELEMENT_DESC* descs, const uint& count, ID3DBlob* blob)
{
	if (!descs || !count || !blob)
		assert(false);

	auto hr = device->CreateInputLayout
	(
		descs,
		count,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&input_layout
	);

	CHECK(hr);
}

void D3D11_InputLayout::Clear()
{
	SAFE_RELEASE(input_layout);
}
