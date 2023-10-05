#include "pch.h"
#include "D3D11_Pipeline.h"

D3D11_Pipeline::D3D11_Pipeline(Graphics* graphics)
{
	device_context = graphics->GetDeviceContext();
}

D3D11_Pipeline::~D3D11_Pipeline()
{
}

bool D3D11_Pipeline::Begin(const D3D11_PipelineState& pipeline_state)
{
	// InputLayout
	{
		if (pipeline_state.input_layout && pipeline_state.input_layout->GetResource())
		{
			device_context->IASetInputLayout(pipeline_state.input_layout->GetResource());
		}
		else
			assert(false);
	}

	// Primitive Topology
	{
		device_context->IASetPrimitiveTopology(pipeline_state.primitive_topology);
	}

	// Vertex Shader
	{
		if (pipeline_state.vertex_shader)
		{
			// 만약에 shader 파일이 존재한다면
			if (auto new_shader = pipeline_state.vertex_shader
				->GetResource())
			{
				uint instance_count			=	256;
				ID3D11ClassInstance* instance[256] = { nullptr, };
				ID3D11VertexShader* current_shader = nullptr;
				device_context->VSGetShader(&current_shader, instance, &instance_count);

				// shader 파일이 변경이 되었다면
				if(current_shader != new_shader)
				{ 
					device_context->VSSetShader(static_cast<ID3D11VertexShader*>(new_shader), nullptr, 0);
				}
			}
		}
	
	}

	//Pixel Shader
	{
		uint instance_count			= 256;
		ID3D11ClassInstance* instance[256] = { nullptr, };
		ID3D11PixelShader* current_shader = nullptr;
		device_context->PSGetShader(&current_shader, instance, &instance_count);

		auto new_shader = pipeline_state.pixel_shader ? pipeline_state.pixel_shader->GetResource() : nullptr;
		if (current_shader != new_shader)
			device_context->PSSetShader(static_cast<ID3D11PixelShader*>(new_shader), nullptr, 0);
	}

	// Rasterizer State
	{
		if (pipeline_state.rasterizer_state && pipeline_state.rasterizer_state->GetResource())
		{
			device_context->RSSetState(pipeline_state.rasterizer_state->GetResource());
		}
	}

	// Blend State
	{
		if (pipeline_state.blend_state && pipeline_state.blend_state->GetResource())
		{
			float factor = pipeline_state.blend_state->GetBlendFactor();
			float blend_factor[] = { factor, factor, factor, factor };
			uint sample_mask = pipeline_state.blend_state->GetSampleMask();

			device_context->OMSetBlendState(pipeline_state.blend_state->GetResource(), blend_factor, sample_mask);
		}
	}
	
	return true;
}

void D3D11_Pipeline::End()
{
	// TODO : 
}

void D3D11_Pipeline::SetVertexBuffer(const D3D11_VertexBuffer* buffer)
{
	if (!buffer || !buffer->GetResource())
	{
		assert(false);
	}

	ID3D11Buffer* buffers[]{ buffer->GetResource(), };
	device_context->IASetVertexBuffers(0, 1, buffers, &buffer->GetStride(), &buffer->GetOffset());
}

void D3D11_Pipeline::SetIndexBuffer(const D3D11_IndexBuffer* buffer)
{
	if (!buffer || !buffer->GetResource())
	{
		assert(false);
	}

	device_context->IASetIndexBuffer(buffer->GetResource(), DXGI_FORMAT_R32_UINT, buffer->GetOffset());
}

void D3D11_Pipeline::SetConstantBuffer(const uint& slot, const uint& scope, const D3D11_ConstantBuffer* buffer)
{
	if (!buffer || !buffer->GetResource())
	{
		assert(false);
	}

	if (slot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
	{
		assert(false);
	}

	ID3D11Buffer* constant_buffers[]{ buffer->GetResource(), };

	if (scope & ShaderScope_VS) device_context->VSSetConstantBuffers(slot, 1, constant_buffers);
	if (scope & ShaderScope_PS) device_context->PSSetConstantBuffers(slot, 1, constant_buffers);
}

void D3D11_Pipeline::SetShaderResource(const uint& slot, const uint& scope, const D3D11_Texture* resource)
{
}

void D3D11_Pipeline::SetSamplerState(const uint& slot, const uint& scope, const D3D11_SamplerState* state)
{
}

void D3D11_Pipeline::Draw(const uint& vertex_count, const uint& vertex_offset)
{
}

void D3D11_Pipeline::DrawIndexed(const uint& index_count, const uint& index_offset, const uint& vertex_offset)
{
}
