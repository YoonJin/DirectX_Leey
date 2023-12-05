#pragma once

class IObserver
{
public:
	IObserver(class Context* context) : context(context) {}

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void ReceivedNotify() = 0;

	auto GetContext() const -> const class Context* const { return context; }
		
protected:
	class Context* context = nullptr;
};