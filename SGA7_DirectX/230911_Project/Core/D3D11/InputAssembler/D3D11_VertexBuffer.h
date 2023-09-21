#pragma once

class D3D11_VertexBuffer final
{
public:
	D3D11_VertexBuffer(class Graphics* graphics);
	~D3D11_VertexBuffer();

	auto GetResource() const { return buffer; }
	auto GetStride()   const -> const uint& { return stride; }
	auto GetOffset()   const -> const uint& { return offset; }
	auto GetCount()    const -> const uint& { return count;  }

	template<typename T>
	void Create(const std::vector<T>& vertices, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE);
	void Clear();

private:
	ID3D11Device* device = nullptr;
	ID3D11Buffer* buffer = nullptr;
	uint stride			 = 0;
	uint offset			 = 0;
	uint count			 = 0;
};

template<typename T>
inline void D3D11_VertexBuffer::Create(const std::vector<T>& vertices, const D3D11_USAGE& usage)
{
	stride = sizeof(T);
	count = static_cast<uint>(vertices.size());

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = stride * count;
	desc.Usage	   = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
		break;
	case D3D11_USAGE_IMMUTABLE: 
		desc.CPUAccessFlags = 0; 
		break;
	case D3D11_USAGE_DYNAMIC:   
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	  
		break;
	case D3D11_USAGE_STAGING:  
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ; 
		break;	
	}

	D3D11_SUBRESOURCE_DATA sub_data;
	ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
	sub_data.pSysMem = vertices.data();

	auto hr = device->CreateBuffer(&desc, &sub_data, &buffer);
	CHECK(hr);
}