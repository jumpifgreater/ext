#pragma once
#include <Windows.h>

bool IsGameRunning(const char* windowTitle) {
	auto hwnd = FindWindow(NULL, windowTitle);
	return hwnd != NULL;
}