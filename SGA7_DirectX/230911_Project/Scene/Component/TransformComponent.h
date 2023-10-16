#pragma once
#include "IComponent.h"

class TransformComponent final : public IComponent
{
public:
	TransformComponent
	(
		class Actor* actor,
		class TransformComponent* transform
	);
	~TransformComponent();

	void Initialize() override;
	void Update()     override;
	void Destroy()    override;

	/*
		Local
	*/
	const Vec3& GetLocalScale() const { return local_scale; }
	void SetLocalScale(const Vec3& scale);

	const Vec3& GetLocalPosition() const { return local_position; }
	void SetLocalPosition(const Vec3& position);

	const Vec3& SetLocalRotation() const { return local_rotation; }
	void SetLocalRotation(const Vec3& rotation);

	const Matrix& GetLocalMatrix() const { return local; }

	/*
		World
	*/
	const Vec3 GetScale();
	void SetSclae(const Vec3& world_scale);
	const Vec3 GetPosition();
	void SetPosition(const Vec3& world_position);
	const Vec3 GetRotation();
	void SetRotation(const Vec3& world_rotation);

	const Matrix GetWorldRotationMatrix() const;
	const Matrix& GetWorldMatrix() const { return world; }

	Vec3 GetRight() const;
	Vec3 GetUp() const;
	Vec3 GetForward() const;

	/*
		Transform
	*/
	bool HasParent() const { return parent ? true : false; }
	bool HasChild() const { return !childs.empty(); }

	TransformComponent* const GetRoot() 
	{ return HasParent ? parent->GetRoot() : this; }
	
	TransformComponent* const GetParent() { return parent; }
	void SetParent(TransformComponent* const new_parent);

	const std::vector<TransformComponent*>& GetChilds() const
	{
		return childs;
	}
	TransformComponent* const GetChildFromIndex(const uint& index);

	uint GetChildCount() const 
	{ return static_cast<uint>(childs.size()); }

	void AddChild(TransformComponent* const child);

	void UpdateTransform();

private:
	Vec3 local_scale = Vec3(1, 1, 1);
	Vec3 local_position = Vec3(0, 0, 0);
	Vec3 local_rotation = Vec3(0, 0, 0);

	Matrix local; // s * r * t
	Matrix world; // local * parent

	TransformComponent* parent = nullptr;
	std::vector<TransformComponent*> childs;
};