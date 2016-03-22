/*----------------------------------------------
DevCaps.c
	Show some information of device context
	code and compile it by Visual Studio 2008
----------------------------------------------*/

#include <windows.h>
#include <tchar.h>

#define NUMLINES ((int) (sizeof devcaps / sizeof devcaps [0]))
 
struct
{
     int     iIndex ;
     TCHAR * szLabel ;
     TCHAR * szDesc ;
}

devcaps [] =
{
     HORZSIZE,      TEXT ("HORZSIZE"),     TEXT ("Width in millimeters:"),
     VERTSIZE,      TEXT ("VERTSIZE"),     TEXT ("Height in millimeters:"),
     HORZRES,       TEXT ("HORZRES"),      TEXT ("Width in pixels:"),
     VERTRES,       TEXT ("VERTRES"),      TEXT ("Height in raster lines:"),
     BITSPIXEL,     TEXT ("BITSPIXEL"),    TEXT ("Color bits per pixel:"),
     PLANES,        TEXT ("PLANES"),       TEXT ("Number of color planes:"),
     NUMBRUSHES,    TEXT ("NUMBRUSHES"),   TEXT ("Number of device brushes:"),
     NUMPENS,       TEXT ("NUMPENS"),      TEXT ("Number of device pens:"),
     NUMMARKERS,    TEXT ("NUMMARKERS"),   TEXT ("Number of device markers:"),
     NUMFONTS,      TEXT ("NUMFONTS"),     TEXT ("Number of device fonts:"),
     NUMCOLORS,     TEXT ("NUMCOLORS"),    TEXT ("Number of device colors:"),
     PDEVICESIZE,   TEXT ("PDEVICESIZE"),  TEXT ("Size of device structure:"),
     ASPECTX,       TEXT ("ASPECTX"),      TEXT ("Relative width of pixel:"),
     ASPECTY,       TEXT ("ASPECTY"),      TEXT ("Relative height of pixel:"),
     ASPECTXY,      TEXT ("ASPECTXY"),     TEXT ("Relative diagonal of pixel:"),
     LOGPIXELSX,    TEXT ("LOGPIXELSX"),   TEXT ("Horizontal dots per inch:"),
     LOGPIXELSY,    TEXT ("LOGPIXELSY"),   TEXT ("Vertical dots per inch:"),
     SIZEPALETTE,   TEXT ("SIZEPALETTE"),  TEXT ("Number of palette entries:"),
     NUMRESERVED,   TEXT ("NUMRESERVED"),  TEXT ("Reserved palette entries:"),
     COLORRES,      TEXT ("COLORRES"),     TEXT ("Actual color resolution:")
} ;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{
	static TCHAR wndClassName[] = TEXT("DevCaps");
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
	hWnd = CreateWindow(wndClassName, TEXT("Get device capability"), 
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
	static int cxChar = 0, cxCaps = 0, cyChar = 0;
	TCHAR szBuffer[10] = {0};
	HDC hdc;
	int i  = 0;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;

	switch(message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily&1?3:2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hWnd, hdc);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for(i = 0; i< NUMLINES; i++)
		{
			TextOut(hdc, 0, cyChar * i, devcaps[i].szLabel, 
					lstrlen(devcaps[i].szLabel));
			TextOut(hdc, 14 * cxCaps, cyChar * i, 
					devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, 14 * cxCaps + 35 * cxChar, cyChar * i ,
					szBuffer, swprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR),
								TEXT("%5d"), GetDeviceCaps(hdc, devcaps[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
					
		}

		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}