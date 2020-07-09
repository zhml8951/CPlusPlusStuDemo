#include <Windows.h>
#include <cstdio>
#include <iostream>

void find_window_demo()
{
	auto win01 = FindWindow(nullptr, TEXT("new 1 - Notepad++"));
	SendMessage(win01, WM_CHAR, WPARAM('A'), 0);
	std::cout << "notepad++_HWND:  " << win01 << "\n";
	auto win02 = FindWindow(TEXT("ConsoleWindowClass"), nullptr);
	std::cout << "notepad++_HWND:  " << win02 << "\n";

	//SendMessage(win02,WM_CLEAR, 0, 0);

	auto win03 = FindWindow(nullptr, TEXT("C:\\PowerShell\\7\\pwsh.exe"));
	SendMessage(win03, WM_CLOSE, 0, 0);
	Sleep(100);
	MessageBox(NULL, TEXT("hehe"), TEXT("text"), MB_OK|MB_YESNO);
	::WinExec("calc.exe", 5);
}


