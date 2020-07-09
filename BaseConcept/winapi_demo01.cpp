#include "winapi_demo01.h"
#include <iostream>
#include <cstdio>
#include <Windows.h>

void test_find_window()
{
	const auto lp_notepad = TEXT("new 1 - Notepad++");
	auto const h_wnd = FindWindow(nullptr, lp_notepad);
	if (h_wnd == nullptr) {
		std::cout << "Notepad++ Not running!";
		return;
	}
	std::cout << "notepad++ h_wnd: " << h_wnd << "\n";
	SetForegroundWindow(h_wnd);
	auto rect = MessageBox(h_wnd, TEXT("Messagebox test01"), TEXT("test01"), MB_YESNO);

	if (rect == IDYES) {
		//auto title = static_cast<char*>(malloc(100 * sizeof(char)));
		//strcpy_s(title, 100*sizeof(char),"Windows api stu01" );
		//title = nullptr;

		//auto const title_ch = "notepad++新文档";
		//WCHAR title[256];
		//memset(title, 0, sizeof(title));
		//MultiByteToWideChar(CP_ACP, 0, title_ch, strlen(title_ch) + 1, title, sizeof(title) / sizeof(title[0]));

		auto title = L"新文档";
		if (!title) {
			MessageBox(nullptr, TEXT("malloc mem error."), TEXT("ERROR!"), MB_OK);
			return;
		}
		std::cout << "title:  " << title << "\n";
		SendMessage(h_wnd, WM_SETTEXT, 0, LPARAM(title));
		//SendMessage(h_wnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(title));
		//free(title);
	}
	else {
		const auto rst = MessageBox(nullptr, TEXT("message"), TEXT("TEST"), MB_OK);
		std::cout << "rst: " << rst << "\n";
	}
}

void wchar_to_char(const wchar_t* wp_char)
{
	char* p_char = nullptr;
	auto len = WideCharToMultiByte(CP_ACP, 0, wp_char, wcslen(wp_char), nullptr, 0, nullptr, nullptr);
	std::cout << "len: " << len << "\n";
	std::wcout << "Inputted wchar_t: " << wp_char << ", *wp_char: " << *wp_char << "\n";
	std::cout << "wchar*_t.sizeof: " << sizeof(wchar_t) << "\n";
	p_char = static_cast<char*>(malloc(len + 1));
	WideCharToMultiByte(CP_ACP, 0, wp_char, wcslen(wp_char), p_char, len, nullptr, nullptr);
	p_char[len] = '\0';
	std::cout << "p_char: " << p_char << ", *p_char: " << *p_char << '\n';
	free(p_char);
}

void win_api_demo_main()
{
	//test_find_window();
	setlocale(LC_ALL, "chs");
	wchar_t test_wchar01[] = L"测试语句01__";
	std::wcout << "test_wch: " << test_wchar01 << "\n";
	wchar_to_char(test_wchar01);
	HMODULE hDll = LoadLibrary(TEXT("..//dynamic.dll"));
}
