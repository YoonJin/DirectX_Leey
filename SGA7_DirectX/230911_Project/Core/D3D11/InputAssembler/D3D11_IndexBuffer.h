#pragma once

class D3D11_IndexBuffer final
{
public:
	D3D11_IndexBuffer(class Graphics* graphics);
	~D3D11_IndexBuffer();

	auto GetResource() const { return buffer;  }
	auto GetStride()   const { return stride;  }
	auto GetOffset()   const { return offset;  }
	auto GetCount()	   const { return count;   }

	void Create(const std::vector<uint>& indices, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE);
	void Clear();

private:
	ID3D11Device* device = nullptr;
	ID3D11Buffer* buffer = nullptr;
	uint		  stride = 0;
	uint		  offset = 0;
	uint		  count  = 0;
};
