#pragma once

// 상수 버퍼(Constant 버퍼에 사용)
// GPU에서 데이터를 처리할 때, 데이터는 종종 특정 바이트로 정렬되어야 한다.
// 그러면서 셰이더에서 이 구조체를 올바르게 해석 할 수 있게 한다.
// Vec3는 3개의 float 값을 포함하므로, 총 12바이트 메모리를 차지하지만,
// 많은 경우 DirectX의 데이터는 16바이트로 메모리에 정렬이 되는 것이 
// 데이터 처리의 효율성을 높일수 있다.
struct TransformData
{
	Matrix matWorld		 = Matrix::Identity;
	Matrix matView		 = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};

class Execute final
{
public:
	Execute();
	~Execute();

	void Update();
	void Render();

private:
	void CreateGeometry();
	void CreateInputLayout();
	void CreateVS();
	void CreatePS();


	void CreateRasterizerState();
	// 텍스처 샘플링 시 어떻게 텍스처 픽셀(texels)을 필터링하고 해석할지.
	// == 텍스처 픽셀 계산을 어떻게 처리할지
	void CreateSamplerState();
	void CreateBlendState();

	// 텍스처 파일 로드
	void CreateSRV();

	void CreateConstantBuffer();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ID3DBlob** blob);

private:
	Graphics* graphics = nullptr;

	// Geometry
	Geometry<VertexTexture> geometry;
	ID3D11Buffer* _vertexBuffer = nullptr;
	ID3D11Buffer* _indexBuffer = nullptr;

	// 버텍스 정보를 어떻게 끊어 읽을지 등에 대한 정보 및 설명서
	ID3D11InputLayout* _inputLayout = nullptr;

	// VS
	ID3D11VertexShader* _vertexShader = nullptr;
	// ID3DBlob : Binary Large Object(Blob)을 나타낸다.
    // 컴파일된 셰이더 코드나 데이터 파일 등의 이진 데이터를 저장하고 조작할 수 있는
    // 기본 데이터 구조이다.
	ID3DBlob* _vsBlob = nullptr;

	// RS
	ID3D11RasterizerState* _rasterizerState = nullptr;


	// PS
	ID3D11PixelShader* _pixelShader = nullptr;
	ID3DBlob* _psBlob = nullptr;

	// SRV
	// 셰이더에서 읽을 수 있는 리소스에 대한 뷰를 제공한다.
	// 이것은 텍스처, 버퍼 등의 GPU 리소스에 대한 읽기 전용 엑세스를 셰이더에 제공하게 된다.
	ID3D11ShaderResourceView* _shaderResourceView = nullptr;
	ID3D11ShaderResourceView* _shaderResourceView2 = nullptr;

	ID3D11SamplerState* _samplerState = nullptr;
	ID3D11BlendState* _blendState = nullptr;

private:
	TransformData _transformData;
	ID3D11Buffer* _constantBuffer;

	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 250.0f, 250.0f, 0.f };
};