#pragma once

class Execute final
{
public:
	Execute();
	~Execute();
	

	void Update();
	void Render();

private:
	class Graphics* graphics = nullptr;
	class Camera* camera = nullptr;
	class D3D11_ConstantBuffer* camera_buffer = nullptr;
	class D3D11_Pipeline* pipeline = nullptr;
	
	// TODO °´Ã¼µé Ãß°¡...

};