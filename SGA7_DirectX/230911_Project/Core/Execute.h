#pragma once

// ��� ����(Constant ���ۿ� ���)
// GPU���� �����͸� ó���� ��, �����ʹ� ���� Ư�� ����Ʈ�� ���ĵǾ�� �Ѵ�.
// �׷��鼭 ���̴����� �� ����ü�� �ùٸ��� �ؼ� �� �� �ְ� �Ѵ�.
// Vec3�� 3���� float ���� �����ϹǷ�, �� 12����Ʈ �޸𸮸� ����������,
// ���� ��� DirectX�� �����ʹ� 16����Ʈ�� �޸𸮿� ������ �Ǵ� ���� 
// ������ ó���� ȿ������ ���ϼ� �ִ�.
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
	// �ؽ�ó ���ø� �� ��� �ؽ�ó �ȼ�(texels)�� ���͸��ϰ� �ؼ�����.
	// == �ؽ�ó �ȼ� ����� ��� ó������
	void CreateSamplerState();
	void CreateBlendState();

	// �ؽ�ó ���� �ε�
	void CreateSRV();

	void CreateConstantBuffer();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ID3DBlob** blob);

private:
	Graphics* graphics = nullptr;

	// Geometry
	Geometry<VertexTexture> geometry;
	ID3D11Buffer* _vertexBuffer = nullptr;
	ID3D11Buffer* _indexBuffer = nullptr;

	// ���ؽ� ������ ��� ���� ������ � ���� ���� �� ����
	ID3D11InputLayout* _inputLayout = nullptr;

	// VS
	ID3D11VertexShader* _vertexShader = nullptr;
	// ID3DBlob : Binary Large Object(Blob)�� ��Ÿ����.
    // �����ϵ� ���̴� �ڵ峪 ������ ���� ���� ���� �����͸� �����ϰ� ������ �� �ִ�
    // �⺻ ������ �����̴�.
	ID3DBlob* _vsBlob = nullptr;

	// RS
	ID3D11RasterizerState* _rasterizerState = nullptr;


	// PS
	ID3D11PixelShader* _pixelShader = nullptr;
	ID3DBlob* _psBlob = nullptr;

	// SRV
	// ���̴����� ���� �� �ִ� ���ҽ��� ���� �並 �����Ѵ�.
	// �̰��� �ؽ�ó, ���� ���� GPU ���ҽ��� ���� �б� ���� �������� ���̴��� �����ϰ� �ȴ�.
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