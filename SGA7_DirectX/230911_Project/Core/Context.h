#pragma once

#include "pch.h"
#include "Subsystem/ISubsystem.h"

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
		static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem");
		return static_cast<T*>(subsystems.emplace_back(new T(this)));
	}

	template <typename T>
	auto GetSubsystem() -> T* const
	{
		static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem");

		for (const auto& subsystem : subsystems)
		{
			// typeid == 런타임에 개체의 형식을 확인할 수 있다.
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
	std::vector<ISubsystem*> subsystems;
};