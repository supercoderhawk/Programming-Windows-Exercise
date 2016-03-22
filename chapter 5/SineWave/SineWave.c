/*----------------------------------------------
SineWave.c
	Show the wave graphics of sine function
	code and compile it by Visual Studio 2008
----------------------------------------------*/

#include <windows.h>
#include <math.h>
#include <tchar.h>

#define NUM 1000
#define TWOPI (2 * 3.141592653)

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{
	static TCHAR wndClassName[] = TEXT("SineWave");
	WNDCLASS wndClass;
	HWND hWnd = NULL;
	MSG msg;

	//define value of windows class
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hInstance = hInstance;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = wndClassName;

	//register the window class
	if(!RegisterClass(&wndClass))
	{
		MessageBox(NULL, TEXT("Register failed"), TEXT("hint"), MB_OKCANCEL);
	}

	//create window
	hWnd = CreateWindow(wndClassName, TEXT("Display Sine Wave"), 
						WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	//set message loop
	while(GetMessage(&msg, NULL, 0, 0 ))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxClient = 0, cyClient = 0;
	HDC hdc;
	int i  = 0;
	PAINTSTRUCT ps;
	POINT apt[NUM];

	switch(message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		MoveToEx(hdc, 0, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);

		for(i = 0; i < NUM; i++)
		{
			apt[i].x = i * cxClient / NUM;
			apt[i].y = (int) (cyClient / 2 * (1 - sin(TWOPI * i / NUM)));
		}
		Polyline(hdc, apt, NUM);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}