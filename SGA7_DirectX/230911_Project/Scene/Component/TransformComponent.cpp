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

// ��ü�� ���� ��Ʈ�������� ������ ������ �����Ͽ� ��ȯ�ϴ� ������ �Ѵ�.
// ���� ��Ʈ������ ��ġ, ȸ��, ������ ������ �ֿ� ��ȯ ��Ҹ� �����ϰ� �ִ�.
const Vec3 TransformComponent::GetScale()
{
	XMVECTOR scale, rotation, translation;

	// �־��� world ��Ʈ������ �����Ͽ�, ��ġ, ȸ��, ��������
	// ���� �����ϴ� �Լ��̴�.
	bool result = 
		XMMatrixDecompose(&scale, &rotation, &translation, world);

	if (!result)
	{
		return Vec3(1.f, 1.f, 1.f);
	}

	Vec3 world_scale;
	// XMVECTOR Ÿ���� Vec3���·� ��ȯ����
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
		// ������� ������ ��İ� ������ �� ��������� �Ǵ� ���
		// 1. �θ��� ��Ʈ ��Ʈ������ ����� : 
		// 3D �׷��Ƚ����� ��ü���� ���� �θ�-�ڽ� ���踦 ���´�.
		// �� ��ü�� �ڽ��� "����" ���������� ��ġ�� ȸ���� ������.
		// �θ��� ���� ��Ʈ������ �θ� ��ü�� ���� ������ ���
		// ��ġ �Ǿ� �ִ����� ��Ÿ���� ��Ʈ�����̴�.
		// �̸� ����ķ� ��ȯ�ϴ� ������, ���� ��ǥ�� 
		// �θ��� ���� ��ǥ�� ��ȯ�ϱ� �����̴�.
		inverse = XMMatrixInverse(nullptr, parent->GetWorldMatrix());

		Vec3 position;

		// �θ���ġ������ ������� ��ġ�� ���� �� �ִ�.
		// �� �Լ��� ����ϸ� 3D ��ǥ�� ��Ʈ������ ��ȯ�Ҷ�, ���� ��ǥ��
		// ��Ʈ������ ��������.
		// ������� ����ϸ�, ���� ���������� ��ǥ�� �θ��� ���� ��������
		// ��ȯ�� �� �ִ�. �̷��� ��ȯ �� ��ǥ�� ���� ����������
		// ���� ��ġ�� �ƴ϶� �θ� ��ü�� ��� ���� ��ġ�� �ȴ�.
		position = XMVector3TransformCoord(world_position, inverse);

		// ��ġ�� �����Ѵ�.
		SetLocalPosition(position);
	}
	else
		SetLocalPosition(world_position);
}

const Vec3 TransformComponent::GetRotation()
{
	XMVECTOR scale, rotationQuaternion, translation;

	// 3D ȸ�� ������ ���ʹϾ����� ����
	XMMatrixDecompose(&scale, &rotationQuaternion, &translation, world);

    // �� ���ʹϾ��� ȸ�� ��Ʈ������ ��ȯ
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuaternion);

	// �ش� ȸ�� ��Ʈ�������� ���Ϸ� ���� ����Ͽ� ��ȯ��.
	// �� DirectXMath������ ��Ʈ���� ������ ������ column-major�� ����
	// �ǹǷ� ���� �ڵ��� row-major ���ٰ� �����ϰ� ���� �����ϱ� ����
	// XMMatrixTranspose �Լ��� ����Ͽ� ��Ʈ������ ��ġ
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
		// �θ���ġ������ ������� ȸ������ ���� �� �ִ�.
		// ȸ�� ���ʹ� ���⸸ �߿��ϹǷ�
		// Coord�� �ƴ� Normal ������ �Լ��� ����Ѵ�. �� �Լ���
		// w ��ǥ�� ��ȯ ���� 3D ������ ���⸸�� ��ȯ�Ѵ�.
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

	// ���ص� ���ʹϾ��� �̿��Ͽ� ȸ�� ����� �����Ѵ�.
	XMMATRIX rotation = XMMatrixRotationQuaternion(rotationQuat);

	// Matrix�� ��ȯ
	XMFLOAT4X4 float4x4;
	XMStoreFloat4x4(&float4x4, rotation);

	// SimpleMatrix ����
	SimpleMath::Matrix simpleMatrix(float4x4);

	return simpleMatrix;
}

// ���� ��Ʈ������ �������� ������ ���� ���͸� ������ �Լ�.
Vec3 TransformComponent::GetRight() const
{
	Vec3 right;
	// ���� ��Ʈ������ �������� ������ ������ �����������
	// ����ؼ� ��ȯ���ִ� ���.
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
