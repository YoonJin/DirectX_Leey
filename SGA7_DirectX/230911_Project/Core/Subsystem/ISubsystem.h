#pragma once

class ISubsystem
{
public:
	ISubsystem(class Context* context) : context(context) {}

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() {}

	auto GetContext() const -> const class Context* const { return context; }
		
protected:
	class Context* context = nullptr;
};