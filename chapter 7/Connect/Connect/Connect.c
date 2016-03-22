/*-----------------------------------------------
Connect.c
	Connect the dots mouse demo program
	code and compile it with Visual Studio 2008
-----------------------------------------------*/

#include <windows.h>
#include <tchar.h>
#define MAXPOINTS 1000

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nShowCmd)
{
	static TCHAR szClassName[] = TEXT("Connect Mouse");
	WNDCLASS wndClass;
	HWND hWnd;
	MSG msg;

	//define the window class value
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hInstance = hInstance;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szClassName;

	//register window class
	if(!RegisterClass(&wndClass))
	{
		MessageBox(NULL, TEXT("register window failed!"), TEXT("hint"), MB_OKCANCEL);
		return 0;
	}

	//create window
	hWnd = CreateWindow(szClassName, TEXT("Connect Window"), 
						WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//set window loop
	while(!GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}
					
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static POINT pt[MAXPOINTS];
	int i = 0, j = 0;
	static int count = 0;

	switch(message)
	{
	case WM_LBUTTONDOWN:
		iCount = 0;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_MOUSEMOVE:

	}
}