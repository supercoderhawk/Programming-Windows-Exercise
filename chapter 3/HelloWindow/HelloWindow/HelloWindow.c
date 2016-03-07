/*----------------------------------------------
HelloWindow.c
	Displays "Hello, Window." in client area
	code and compile it with visual studio 2008

----------------------------------------------*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int iCmdShow)
{
	static TCHAR szWndClassName[] = TEXT("Hello Window");
	HWND hwnd;
	HWND hwnd2;
	MSG  msg;
	WNDCLASS wndClass;

	//define property of window class
	wndClass.style = CS_HREDRAW|CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szWndClassName;

	//register window class
	if(!RegisterClass(&wndClass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), 
					TEXT("Hint"), MB_ICONERROR|MB_OKCANCEL);
		return 0;
	}

	//create the window based on the previous registered window class
	hwnd = CreateWindow(szWndClassName,				//window class name
						TEXT("The Hello Program"),	//window caption
						WS_OVERLAPPEDWINDOW,		//window style
						CW_USEDEFAULT,				//initial x position
						CW_USEDEFAULT,				//initial y position
						CW_USEDEFAULT,				//initial x size
						CW_USEDEFAULT,				//initial y size
						NULL,						//parent window handle
						NULL,						//window menu handle
						hInstance,					//program instance handle
						NULL);						//creation parameters

	//show and refresh the window
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	//set message loop
	while(GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return msg.wParam;
}

//window process function
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;
	RECT		rect;

	switch(message)
	{
	case WM_CREATE:
		PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);

		DrawText(hdc, TEXT("Hello, Window."), -1, &rect, 
				DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		EndPaint(hwnd, &ps);
		return 0;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}