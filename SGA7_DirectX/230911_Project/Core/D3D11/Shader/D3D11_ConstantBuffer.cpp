#include "pch.h"
#include "D3D11_ConstantBuffer.h"

D3D11_ConstantBuffer::D3D11_ConstantBuffer(Graphics* graphics)
{
	device		   = graphics->GetDevice();
	device_context = graphics->GetDeviceContext();
}

D3D11_ConstantBuffer::~D3D11_ConstantBuffer()
{
	Clear();
}

void D3D11_ConstantBuffer::Clear()
{
	SAFE_RELEASE(buffer);
}

bool D3D11_ConstantBuffer::Unmap()
{
	if (!device || !device_context || !buffer)
		return false;

	device_context->Unmap(buffer, 0);
	
	return true;
}
