#include <windows.h>
#include <fstream>
#include <sys/stat.h>
HHOOK hKeyboardHook = NULL;
int prev = 0;
int key;
DWORD dwMsg;
KBDLLHOOKSTRUCT hooked_key;
MSG message;
HINSTANCE hins;
std::ofstream file;
inline bool exists(const std::string &name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
	{
		hooked_key = *((KBDLLHOOKSTRUCT *)lParam);
		dwMsg = 1;
		dwMsg = dwMsg + hooked_key.scanCode << 16;
		dwMsg = dwMsg + hooked_key.flags << 24;
		key = hooked_key.vkCode;
		if (key == 36)
		{
			if (prev == 162)
			{
				ShellExecute(NULL, "open", "C:\\Users\\Public\\change_desktop.bat", NULL, NULL, SW_SHOW);
			}
		}
		prev = key;
	}
	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}
int main()
{
	if (!exists("C:\\Users\\Public\\change_desktop.bat"))
	{
		file.open("C:\\Users\\Public\\change_desktop.bat");
		file << "explorer.exe shell:::{3080F90E-D7AD-11D9-BD98-0000947B0257}";
		file.close();
	}
	hins = GetModuleHandle(NULL);
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LowLevelKeyboardProc, hins, 0);
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}