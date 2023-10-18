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
			this,				//  현재 Actor를 가리키는 포인터
			transform.get()	    //  raw pointer로 반환된 transformComponent 
		)
	);

	// 마지막으로 추가된 컴포넌트를 가져와서 T 타입을 캐스팅
	auto new_component = std::static_pointer_cast<T>(components.back());
	new_component->Initialize();
	new_component->SetComponentType(type);

	// 만약 T TransformComponent와 동일한 타입이라면,
	// transform 멤버 변수에 할당을 하겠다.
	
    // constexpr == if문이 참인지 거짓인지 확인후 참이라면 컴파일
	// 템플릿 클래스나 함수 내에서 특정 타입에 대한 특별한 처리가 필요할 때
	// 유용하게 사용된다.
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
	// 범위 기반 포문
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
		{
			// C++의 스마트 포인터 관련 유틸리티 함수 중 하나로,
			// 특히 std::shard_ptr을 대상으로 한다.
			// 이 함수는 std::shared_ptr의 기존 타입을 다른 타입으로
			// 변환하기 위해 사용된다.
			// 기본 적으로 static_cast 연산을 std::shared_ptr에 적용
			// 하는 것으로 생각할 수 있다.
			// 만약 변환 대상 타입이 원래 타입과 호환되지 않는 경우
			// 컴파일 오류를 불러 일으켜준다.
			/*
				std::shared_ptr<Base> basePtr =
				std::make_shared<Derived>();

				// basePtr을 Derived로 안전하게 캐스팅
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
