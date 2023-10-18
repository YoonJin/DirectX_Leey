#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(Actor* actor, TransformComponent* transform)
	:IComponent(actor, transform)
{
	local = Matrix::Identity;
	world = Matrix::Identity;
	UpdateTransform();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Initialize()
{
}

void TransformComponent::Update()
{
}

void TransformComponent::Destroy()
{
}

void TransformComponent::SetLocalScale(const Vec3& scale)
{
	if (this->local_scale == scale)
		return;

	this->local_scale = scale;

	UpdateTransform();
}

void TransformComponent::SetLocalPosition(const Vec3& position)
{
	if (this->local_position == position)
		return;

	this->local_position = position;
	UpdateTransform();
}

void TransformComponent::SetLocalRotation(const Vec3& rotation)
{
	if (this->local_rotation == rotation)
		return;

	this->local_rotation = rotation;
	UpdateTransform();
}

// 객체의 월드 매트릭스에서 스케일 정보를 추출하여 반환하는 역할을 한다.
// 월드 매트릭스는 위치, 회전, 스케일 세가지 주요 변환 요소를 포함하고 있다.
const Vec3 TransformComponent::GetScale()
{
	XMVECTOR scale, rotation, translation;

	// 주어진 world 매트릭스를 분해하여, 위치, 회전, 스케일을
	// 각각 추출하는 함수이다.
	bool result = 
		XMMatrixDecompose(&scale, &rotation, &translation, world);

	if (!result)
	{
		return Vec3(1.f, 1.f, 1.f);
	}

	Vec3 world_scale;
	// XMVECTOR 타입을 Vec3형태로 변환해줌
	XMStoreFloat3(&world_scale, scale);

	return world_scale;
}

void TransformComponent::SetSclae(const Vec3& world_scale)
{
	if (GetScale() == world_scale)
		return;

	if (HasParent())
	{
		Vec3 scale;
		Vec3 parent_scale = parent->GetScale();
		scale.x = world_scale.x / parent_scale.x;
		scale.y = world_scale.y / parent_scale.y;
		scale.z = world_scale.z / parent_scale.z;

		SetLocalScale(scale);
	}
	else
		SetLocalScale(world_scale);
}

const Vec3 TransformComponent::GetPosition()
{
	XMVECTOR scale, rotation, translation;

	bool result = XMMatrixDecompose(&scale, &rotation, &translation, world);

	if (!result)
	{
		return Vec3(0.f, 0.f, 0.f);
	}

	Vec3 world_position;
	XMStoreFloat3(&world_position, translation);
	
	return world_position;
}

void TransformComponent::SetPosition(const Vec3& world_position)
{
	if (GetPosition() == world_position)
		return;

	if (HasParent())
	{
		Matrix inverse;
		// 역행렬은 원래의 행렬과 곱했을 때 단위행렬이 되는 행렬
		// 1. 부모의 월트 매트릭스와 역행렬 : 
		// 3D 그래픽스에서 객체들은 종종 부모-자식 관계를 갖는다.
		// 각 객체는 자신의 "로컬" 공간에서의 위치와 회전을 가진다.
		// 부모의 월드 매트릭스는 부모 객체가 월드 공간에 어떻게
		// 배치 되어 있는지를 나타내는 매트릭스이다.
		// 이를 역행렬로 변환하는 이유는, 월드 좌표를 
		// 부모의 로컬 좌표로 변환하기 위함이다.
		inverse = XMMatrixInverse(nullptr, parent->GetWorldMatrix());

		Vec3 position;

		// 부모위치에서의 상대적인 위치를 얻을 수 있다.
		// 이 함수를 사용하면 3D 좌표를 매트릭스로 변환할때, 원래 좌표가
		// 매트릭스와 곱해진다.
		// 역행렬을 사용하면, 월드 공간에서의 좌표를 부모의 로컬 공간으로
		// 변환할 수 있다. 이렇게 변환 된 좌표는 월드 공간에서의
		// 절대 위치가 아니라 부모 객체에 상대 적인 위치가 된다.
		position = XMVector3TransformCoord(world_position, inverse);

		// 위치를 적용한다.
		SetLocalPosition(position);
	}
	else
		SetLocalPosition(world_position);
}

const Vec3 TransformComponent::GetRotation()
{
	XMVECTOR scale, rotationQuaternion, translation;

	// 3D 회전 정보를 쿼터니언으로 추출
	XMMatrixDecompose(&scale, &rotationQuaternion, &translation, world);

    // 그 쿼터니언을 회전 매트릭스로 변환
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuaternion);

	// 해당 회전 매트릭스에서 오일러 각을 계산하여 반환함.
	// 단 DirectXMath에서는 매트릭스 데이터 순서가 column-major로 구성
	// 되므로 원래 코드의 row-major 접근과 동일하게 행을 접근하기 위해
	// XMMatrixTranspose 함수를 사용하여 매트릭스를 전치
	float m31 = XMVectorGetZ(XMMatrixTranspose(rotationMatrix).r[1]);
	float m33 = XMVectorGetZ(XMMatrixTranspose(rotationMatrix).r[2]);
	float m32 = -XMVectorGetY(XMMatrixTranspose(rotationMatrix).r[2]);
	float m12 = XMVectorGetY(XMMatrixTranspose(rotationMatrix).r[0]);
	float m22 = -XMVectorGetY(XMMatrixTranspose(rotationMatrix).r[1]);

	return Vec3
	(
		atan2f(m31, m33),
		atan2f(m32, sqrtf(m12 * m12 + m22 * m22)),
		atan2f(m12, m22)
	);
}

void TransformComponent::SetRotation(const Vec3& world_rotation)
{
	if (GetRotation() == world_rotation)
		return;

	if (HasParent())
	{
		Matrix inverse;

		inverse = XMMatrixInverse(nullptr, GetWorldRotationMatrix());

		Vec3 rotation;
		// 부모위치에서의 상대적인 회전값을 얻을 수 있다.
		// 회전 벡터는 방향만 중요하므로
		// Coord가 아닌 Normal 버전의 함수를 사용한다. 이 함수는
		// w 좌표의 변환 없이 3D 벡터의 방향만을 변환한다.
		rotation = XMVector3TransformNormal(world_rotation, inverse);

		SetLocalRotation(rotation);
	}
	else
		SetLocalRotation(world_rotation);
}

const Matrix TransformComponent::GetWorldRotationMatrix() const
{
	XMVECTOR scale, rotationQuat, translation;

	XMMatrixDecompose(&scale, &rotationQuat, &translation, world);

	// 분해된 쿼터니언을 이용하여 회전 행렬을 생성한다.
	XMMATRIX rotation = XMMatrixRotationQuaternion(rotationQuat);

	// Matrix로 변환
	XMFLOAT4X4 float4x4;
	XMStoreFloat4x4(&float4x4, rotation);

	// SimpleMatrix 생성
	SimpleMath::Matrix simpleMatrix(float4x4);

	return simpleMatrix;
}

// 월드 매트릭스를 기준으로 오른쪽 방향 벡터를 얻어오는 함수.
Vec3 TransformComponent::GetRight() const
{
	Vec3 right;
	// 월드 매트릭스를 기준으로 오른쪽 방향이 어느쪽인지를
	// 계산해서 반환해주는 기능.
	right = XMVector3TransformCoord(Vec3(1, 0, 0), world);
	
	return right;
}

Vec3 TransformComponent::GetUp() const
{
	Vec3 up;
	up = XMVector3TransformCoord(Vec3(0, 1, 0), world);

	return up;
}

Vec3 TransformComponent::GetForward() const
{
	Vec3 forward;
	forward = XMVector3TransformCoord(Vec3(0, 0, 1), world);

	return forward;
}

void TransformComponent::SetParent(TransformComponent* const new_parent)
{
	parent = new_parent;

	new_parent->AddChild(this);
}

TransformComponent* const TransformComponent::GetChildFromIndex(const uint& index)
{
	if (!HasChild())
		return nullptr;

	if (index >= GetChildCount())
		return nullptr;

	return childs[index];
}

void TransformComponent::AddChild(TransformComponent* const child)
{
	childs.push_back(child);
}

void TransformComponent::UpdateTransform()
{
	Matrix S, R, T;
	
	S = Matrix::CreateScale(local_scale.x, local_scale.y, local_scale.z);

	R = Matrix::CreateRotationX(local_rotation.x);
	R *= Matrix::CreateRotationY(local_rotation.y);
	R *= Matrix::CreateRotationZ(local_rotation.z);

	T = Matrix::CreateTranslation(local_position.x, local_position.y, local_position.z);

	local = S * R * T;

	if (HasParent())
		world = local * parent->GetWorldMatrix();
	else
		world = local;

	for (const auto& child : childs)
		child->UpdateTransform();
}

void TransformComponent::UpdateConstantBuffer()
{
	if (!gpu_buffer)
	{
		gpu_buffer = std::make_shared<D3D11_ConstantBuffer>(&Graphics::Get());
		gpu_buffer->Create<TransformBuffer>();
	}

	auto gpu_data = gpu_buffer->Map<TransformBuffer>();
	{
		TransformBuffer cpuData;
		cpuData.matWorld = world;

		memcpy(gpu_data, &cpuData, sizeof(TransformBuffer));
	}
	gpu_buffer->Unmap();
}
