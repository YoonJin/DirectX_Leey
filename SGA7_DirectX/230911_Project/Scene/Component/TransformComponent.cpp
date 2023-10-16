#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(Actor* actor, TransformComponent* transform)
	:IComponent(actor, transform)
{
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

	if (HasParent)
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
	return Matrix();
}

Vec3 TransformComponent::GetRight() const
{
	return Vec3();
}

Vec3 TransformComponent::GetUp() const
{
	return Vec3();
}

Vec3 TransformComponent::GetForward() const
{
	return Vec3();
}

void TransformComponent::SetParent(TransformComponent* const new_parent)
{
}

TransformComponent* const TransformComponent::GetChildFromIndex(const uint& index)
{
	return nullptr;
}

void TransformComponent::AddChild(TransformComponent* const child)
{
}

void TransformComponent::UpdateTransform()
{
}
