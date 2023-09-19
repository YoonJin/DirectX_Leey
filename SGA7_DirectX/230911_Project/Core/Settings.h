#pragma once
#include "pch.h"

// 전역 변수 핸들, 윈도우 사이즈등을 받아오기 위한 클래스
class Settings
{
public:
	static Settings& Get()
	{
		static Settings instance;
		return instance;
	}

	HWND GetWindowHandle() { return handle; }
	void SetWindowHandle(HWND handle) { this->handle = handle; }

	float GetWidth() { return width; }
	void SetWidth(float width) { this->width = width; }

	float GetHeight() { return height; }
	void SetHeight(float height) { this->height = height; }

private:
	HWND handle = nullptr;
	float width = 0.f;
	float height = 0.f;
};