#pragma once

enum class ComponentType : uint
{
	Unknown,
	Camera,
	Transform,
	MeshRenderer,
	MoveScript,
	AIScript,
	Animator,
	Texture
};

class IComponent
{
public:
	template<typename T>
	static constexpr ComponentType DeduceComponentType();

public:
	IComponent
	(
		class Context* const context,
		class Actor* const actor,
		class TransformComponent* const transform
	);
	virtual ~IComponent() = default;

	virtual void Initialize() = 0;
	virtual void Update()  = 0;
	virtual void Destroy() = 0;

	auto GetActor() const { return actor; }
	void SetActor(class Actor* const actor) { this->actor = actor; }

	auto GetTransform() const { return transform; }
	void SetTransform(class TransformComponent* const transform) 
	{
		this->transform = transform; 
	}

	auto GetComponentType() const { return component_type; }
	void SetComponentType(const ComponentType& type)
	{
		this->component_type = type;
	}

	auto IsEnabled() const { return is_enabled; }
	void SetEnabled(const bool& is_enabled)
	{
		this->is_enabled = is_enabled;
	}

protected:
	class Context* context              = nullptr;
	class Actor* actor                  = nullptr;
	class TransformComponent* transform = nullptr;
	ComponentType component_type        = ComponentType::Unknown;
	bool is_enabled                     = true;
};

