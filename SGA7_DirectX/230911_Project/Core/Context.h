#pragma once

#include "pch.h"
#include "Subsystem/IObserver.h"

class Context final
{
public:
	Context() = default;
	~Context()
	{
		for (auto iter = subsystems.begin(); iter != subsystems.end(); iter++)
			SAFE_DELETE(*iter);
	}

	auto Initialize_subsystems() -> const bool
	{
		auto result = true;
		for (const auto& subsystem : subsystems)
		{
			if (!subsystem->Initialize())
				result = false;
		}

		return result;
	}

	template <typename T>
	auto Register_subsystem() -> T* const
	{
		static_assert(std::is_base_of<IObserver, T>::value, "Provided type does not implement ISubsystem");
		return static_cast<T*>(subsystems.emplace_back(new T(this)));
	}

	template <typename T>
	auto GetSubsystem() -> T* const
	{
		static_assert(std::is_base_of<IObserver, T>::value, "Provided type does not implement ISubsystem");

		for (const auto& subsystem : subsystems)
		{
			// typeid == ��Ÿ�ӿ� ��ü�� ������ Ȯ���� �� �ִ�.
			if (typeid(T) == typeid(*subsystem))
				return static_cast<T*>(subsystem);
		}
		return nullptr;
	}

	void Update_subsystems()
	{
		for (const auto& subsystem : subsystems)
			subsystem->Update();
	}

private:
	std::vector<IObserver*> subsystems;
};