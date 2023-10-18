#pragma once
#include "Component/IComponent.h"

class Actor final
{
public:
	Actor();
	~Actor();

	void Initialize();
	void Update();
	void Render(class D3D11_Pipeline* const pipeline);
	void Destroy();

	// property
	auto GetName() const -> const std::string& { return name; }
	void SetName(const std::string& name) { this->name = name; }

	auto IsActive() const { return is_active; }
	void SetActive(const bool& is_active) 
	{
		this->is_active = is_active; 
	}

	auto GetTransform() const -> const std::shared_ptr<class TransformComponent>& { return transform; }
	auto GetTransform_Raw() const -> class TransformComponent*
	{
		return transform.get();
	}

	// Component
	auto AddComponent(const ComponentType& type) -> const std::shared_ptr<IComponent>;

	template <typename T>
	auto AddComponent() -> const std::shared_ptr<T>;

	template <typename T>
	auto GetComponent() -> const std::shared_ptr<T>;

	template <typename T>
	auto GetComponent_Raw() -> T*;

	template <typename T>
	auto GetComponents() -> const std::vector<std::shared_ptr<T>>;

	auto GetAllComponents() const -> const std::vector<std::shared_ptr<IComponent>>& 
	{
		return components;
	}

	bool HasComponent(const ComponentType& type);

	template <typename T>
	bool HasComponent();

	template <typename T>
	void RemoveComponent();


private:
	std::string name;
	bool is_active = true;

	std::shared_ptr<class TransformComponent> transform;
	std::vector<std::shared_ptr<IComponent>> components;
};

template<typename T>
inline auto Actor::AddComponent() -> const std::shared_ptr<T>
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();

	if (HasComponent(type))
		return GetComponent<T>();

	components.push_back
	(
		std::make_shared<T>
		(
			this,				//  ���� Actor�� ����Ű�� ������
			transform.get()	    //  raw pointer�� ��ȯ�� transformComponent 
		)
	);

	// ���������� �߰��� ������Ʈ�� �����ͼ� T Ÿ���� ĳ����
	auto new_component = std::static_pointer_cast<T>(components.back());
	new_component->Initialize();
	new_component->SetComponentType(type);

	// ���� T TransformComponent�� ������ Ÿ���̶��,
	// transform ��� ������ �Ҵ��� �ϰڴ�.
	
    // constexpr == if���� ������ �������� Ȯ���� ���̶�� ������
	// ���ø� Ŭ������ �Լ� ������ Ư�� Ÿ�Կ� ���� Ư���� ó���� �ʿ��� ��
	// �����ϰ� ���ȴ�.
	if constexpr (std::is_same<T, class TransformComponent>::value)
		transform = new_component;

	return new_component;
}

template<typename T>
inline auto Actor::GetComponent() -> const std::shared_ptr<T>
{
	static_assert(std::is_base_of<IComponent, T>::value, 
		"Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();
	// ���� ��� ����
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
		{
			// C++�� ����Ʈ ������ ���� ��ƿ��Ƽ �Լ� �� �ϳ���,
			// Ư�� std::shard_ptr�� ������� �Ѵ�.
			// �� �Լ��� std::shared_ptr�� ���� Ÿ���� �ٸ� Ÿ������
			// ��ȯ�ϱ� ���� ���ȴ�.
			// �⺻ ������ static_cast ������ std::shared_ptr�� ����
			// �ϴ� ������ ������ �� �ִ�.
			// ���� ��ȯ ��� Ÿ���� ���� Ÿ�԰� ȣȯ���� �ʴ� ���
			// ������ ������ �ҷ� �������ش�.
			/*
				std::shared_ptr<Base> basePtr =
				std::make_shared<Derived>();

				// basePtr�� Derived�� �����ϰ� ĳ����
				std::shared_ptr<Derived> derivedPtr =
				std::static_pointer_cast<Derived>(basePtr);
			*/
			return std::static_pointer_cast<T>(component);
		}
	}
	return nullptr;
}

template<typename T>
inline auto Actor::GetComponent_Raw() -> T*
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
			return std::static_pointer_cast<T>(component).get();
	}

	return nullptr;
}

template<typename T>
inline auto Actor::GetComponents() -> const std::vector<std::shared_ptr<T>>
{
	static_assert(std::is_base_of<IComponent, T>::value, 
		"Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();

	std::vector<std::shared_ptr<T>> temp_components;

	for (const auto& component : components)
	{
		if (component->GetComponentType() != type)
			continue;

		temp_components.push_back(std::static_pointer_cast<T>(component));
	}
	return temp_components;
}

template<typename T>
inline bool Actor::HasComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value,
		"Provider type does not implement IComponent");

	return HasComponent(IComponent::DeduceComponentType<T>());
}

template<typename T>
inline void Actor::RemoveComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value,
		"Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();

	for (auto iter = components.begin(); iter != components.end(); )
	{
		auto component = *iter;
		if (component->GetComponentType() == type)
		{
			component->Destroy();
			component.reset();
			iter = components.erase(iter);
		}
		else
			iter++;
	}
}
