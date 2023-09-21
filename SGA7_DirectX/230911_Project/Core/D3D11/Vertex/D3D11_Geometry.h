#pragma once
#include "pch.h"

template <typename T>
class D3D11_Geometry
{
public:
	D3D11_Geometry() = default;
	virtual ~D3D11_Geometry();

	//============================================================
	// [Vertex]
	//============================================================
	auto GetVertexCount()  const { return static_cast<uint>(vertices.size()); }
	auto GetVertexByteWidth() const { return GetVertexCount() * sizeof(T); }
	auto GetVertexPointer()	  const { return vertices.data(); }
	auto GetVertices()		  const { return vertices; }
	auto GetVertices(const uint& offset, const uint& count);

	void AddVertex(const T& vertex);
	void AddVertices(const std::vector<T>& vertices);
	void SetVertices(const std::vector<T>& vertices);

	//============================================================
	// [Index]
	//============================================================
	auto GetIndexCount()	    const { return static_cast<uint>(indices.size()); }
	auto GetIndexByteWidth()	const { return GetIndexCount() * sizeof(uint); }
	auto GetIndexPointer()		const { return indices.data();  }
	auto GetIndices()		    const { return indices; }
	auto GetIndices(const uint& offset, const uint& count);

	void AddIndex(const uint& index);
	void AddIndices(const std::vector<uint>& indices);
	void SetIndices(const std::vector<uint>& indices);


	void Clear();

protected:
	std::vector<T> vertices;
	std::vector<uint> indices;
};
#include "D3D11_Geometry.inl"