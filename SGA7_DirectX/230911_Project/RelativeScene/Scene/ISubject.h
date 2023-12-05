#pragma once

class ISubject
{
public:
	ISubject() = default;
	~ISubject() = default;

	virtual void registerObserver(class IObserver* const observer) = 0;
	virtual void removeObserver(class IObserver* const observer) = 0;
	virtual void notifyObserver() = 0;

};