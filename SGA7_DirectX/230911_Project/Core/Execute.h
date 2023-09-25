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

private:
	Graphics* graphics = nullptr;

	// Geometry
	D3D11_Geometry<D3D11_VertexTexture> geometry;
	D3D11_VertexBuffer* _vertexBuffer = nullptr;
	D3D11_IndexBuffer* _indexBuffer   = nullptr;
	// ���ؽ� ������ ��� ���� ������ � ���� ���� �� ����
	D3D11_InputLayout* _inputLayout   = nullptr;
	D3D11_Shader* _vertexShader = nullptr;
	D3D11_Shader* _pixelShader = nullptr;
	D3D11_ConstantBuffer* _gpuBuffer = nullptr;
	
	// RS
	ID3D11RasterizerState* _rasterizerState = nullptr;

	// SRV
	// ���̴����� ���� �� �ִ� ���ҽ��� ���� �並 �����Ѵ�.
	// �̰��� �ؽ�ó, ���� ���� GPU ���ҽ��� ���� �б� ���� �������� ���̴��� �����ϰ� �ȴ�.
	ID3D11ShaderResourceView* _shaderResourceView = nullptr;
	ID3D11ShaderResourceView* _shaderResourceView2 = nullptr;

	ID3D11SamplerState* _samplerState = nullptr;
	ID3D11BlendState* _blendState = nullptr;

private:
	TransformData _transformData;

	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 250.0f, 250.0f, 0.f };
};