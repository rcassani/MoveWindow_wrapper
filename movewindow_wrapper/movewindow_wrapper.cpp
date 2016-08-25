// movewindow_wrapper.cpp : Defines the entry point for the console application.

#include <iostream>
#include <windows.h>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>


namespace std {
#if defined _UNICODE || defined UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif
}

using namespace std;
int x_pos, y_pos = 0;
wstring wnd_reference = L"";

BOOL CALLBACK enumWindowsProc(__in  HWND hWnd,__in  LPARAM lParam) 
{
	RECT rect;
	wstring minimized_str;
	BOOL move_window = FALSE;
	wchar_t *remain;
	LONG h_wnd;
	
	//If the window is not visible, it is not listed
	if (IsWindowVisible(hWnd) == 0)
	{
		return TRUE;
	}

	// If iconic != 0, the window is minimized
	if (IsIconic(hWnd) != 0)
	{minimized_str = L"Minimized";}
	else{minimized_str = L"NO Minimized";}
	
	int length = ::GetWindowTextLength(hWnd);
	if (0 == length) return TRUE;

	TCHAR* buffer;
	buffer = new TCHAR[length + 1];
	memset(buffer, 0, (length + 1) * sizeof(TCHAR));

	GetWindowText(hWnd, buffer, length + 1);
	wstring windowTitle = wstring(buffer);
	delete[] buffer;
	wprintf(L"%10d\t%s\t%s\r\n", hWnd, minimized_str.c_str(), windowTitle.c_str());
	
	if (wnd_reference.length() != 0) //Check that wnd_reference was provided
	{
		h_wnd = wcstol(wnd_reference.c_str(), &remain, 10); //wnd_reference to long
		if (wstring(remain).length() == 0) //wnd_reference is the (long)hWnd
		{
			if (h_wnd == (long)hWnd)
			{
				move_window = TRUE;
			}
		}
		else //wnd_reference is a Name
		{
			if (windowTitle.compare(0, wnd_reference.length(), wnd_reference) == 0)
			{
				move_window = TRUE;
			}
		}

		if (move_window)
		{
			wprintf(L"Moving window: \"%s\" to X:%d, Y:%d\r\n", windowTitle.c_str(), x_pos, y_pos);
			ShowWindow(hWnd, SW_RESTORE);
			GetWindowRect(hWnd, &rect);
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			MoveWindow(hWnd, x_pos, y_pos, width, height, TRUE);
			SwitchToThisWindow(hWnd, FALSE);
		}
	}
	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[]) {
	if (argc == 4)
	{
		wnd_reference = argv[1];
		x_pos = stoi(argv[2]);
		y_pos = stoi(argv[3]);
	}
	
	wprintf(L"Enumerating Visible Windows...\r\n\n");
	BOOL enumeratingWindowsSucceeded = ::EnumWindows(enumWindowsProc, NULL);
	return 0;
}

