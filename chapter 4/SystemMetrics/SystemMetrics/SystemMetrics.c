/*--------------------------------------------------
SystemMetrics.c
	Display system metrics in a window with a 
horizontal and vertical scroll bar.
	code and compile it with Visual Studio 2008
--------------------------------------------------*/

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#ifndef _SYSTEMMETRICS_H_
#include "SystemMetrics.h"
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nShowCmd)
{
	static TCHAR szWndClassName[] = TEXT("SystemMetrics");
	WNDCLASS wndClass;
	HWND hWnd;
	MSG msg;

	//define the windows class structure value
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szWndClassName;

	//register the window class
	if(!RegisterClass(&wndClass))
	{
		MessageBox(NULL, TEXT("This program needs Windows NT."), 
					szWndClassName, MB_OKCANCEL);
		return 0;
	}

	//create window
	hWnd = CreateWindow(szWndClassName, TEXT("Get System Metrics"), 
						WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	//show and update window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//set message loop
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//window process function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	PAINTSTRUCT ps;

	switch(message)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc= BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_HSCROLL:
		break;
	case WM_VSCROLL:
		break;
	case WM_CLOSE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}