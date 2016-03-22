/*--------------------------------------------------
KeyView.c
	Diplay keyboard and character messages.
	code and compile it with Visual Studio 2008
--------------------------------------------------*/

#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{
	static TCHAR szClassName[] = TEXT("show key strike");
	WNDCLASS wndClass;
	MSG msg;
	HWND hWnd = NULL;
	long er;

	ZeroMemory(&wndClass, sizeof(WNDCLASS));
	ZeroMemory(&msg, sizeof(MSG));

	//define the window class
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hInstance = hInstance;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szClassName;

	//register the window class
	if(!RegisterClass(&wndClass))
	{
		MessageBox(NULL, TEXT("Register the window class failed."),
					TEXT("hint"), MB_OKCANCEL);
		return 0;
	}

	//create the window
	hWnd = CreateWindow(szClassName, TEXT("Show Key Strike"), 
						WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
						NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//set message loop
	while(!GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	er = GetLastError();
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	static int cxClient = 0, cyClient = 0, cxClientMax = 0, cyClientMax = 0;
	static int cxChar = 0, cyChar = 0, cLinesMax = 0, cLines = 0;
	static PMSG pmsg = NULL;
	static RECT rectScroll;
	static TCHAR szTop[] = TEXT("Message Key Char")
							TEXT("Repeat Scan Ext ALT Prev Tran");
	static TCHAR szUnd[] = TEXT("_______ ___ ____")
							TEXT("______ ____ ___ ___ ____ ____");
	static TCHAR* szFormat[2] = {
								TEXT("%-13s %3d %-15s%c%6u %4u %3s %3s %4s %4s"),
								TEXT("%-13s            0x%04x%1s%c %6u %4d %3s %3s %4s %4s")};
	static TCHAR szYes[] = TEXT("Yes");
	static TCHAR szNo[] = TEXT("No");
	static TCHAR szDown[] = TEXT("Down");
	static TCHAR szUp[] = TEXT("Up");
	static TCHAR* szMessage[] = {
								TEXT("WM_KEYDOWN"),			TEXT("WM_KEYUP"),
								TEXT("WM_CHAR"),			TEXT("WM_DEADCHAR"),
								TEXT("WM_SYSKEYDOWN"),		TEXT("WM_SYSKEYUP"),
								TEXT("WM_SYSCHAR"),			TEXT("WM_SYSDEADCHAR")};


	HDC hdc = NULL;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	int i = 0, iType = 0;
	TCHAR szBuffer[128] = {0}, szKeyName[32] = {0};

	switch(message)
	{
	case WM_CREATE:
	case WM_DISPLAYCHANGE:
		//set the client area by resolution
		cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
		cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);

		hdc = GetDC(hWnd);
		
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hWnd, hdc);
		
		if(pmsg)
			free(pmsg);

		cLinesMax = cyClientMax / cyChar;
		pmsg = malloc(cLinesMax * sizeof(MSG));
		ZeroMemory(pmsg, cLinesMax * sizeof(MSG));
		cLines = 0;
		
	case WM_SIZE:
		if(message == WM_SIZE)
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}

		//calculate scrolling rectangle
		rectScroll.left = 0;
		rectScroll.right = cxClient;
		rectScroll.top = cyChar;
		rectScroll.bottom = cyChar * (cyClient / cyChar);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:

		//rearrange storage array
		for(i = cLinesMax - 1; i > 0; i--)
		{
			pmsg[i] = pmsg[i - 1];
		}

		//store new message
		pmsg[0].hwnd = hWnd;
		pmsg[0].message = message;
		pmsg[0].wParam = wParam;
		pmsg[0].lParam = lParam;

		cLines = min(cLines + 1, cLinesMax);

		//scroll up the display
		ScrollWindow(hWnd, 0, -cyChar, &rectScroll, &rectScroll);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
		TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));

		for(i = 0; i < min(cLines, cyClient / cyChar - 1); i++)
		{
			iType = pmsg[i].message == WM_CHAR || pmsg[i].message == WM_SYSCHAR ||
					pmsg[i].message == WM_DEADCHAR ||
					pmsg[i].message == WM_SYSDEADCHAR;

			GetKeyNameText(pmsg[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));

			TextOut(hdc, 0, (cyClient / cyChar -1 - i ) * cyChar, szBuffer,
					swprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), 
								szFormat[iType], szMessage[pmsg[i].message - WM_KEYFIRST],
								pmsg[i].wParam, (PTSTR)(iType ? TEXT("  ") : szKeyName), 
								(TCHAR) (iType ? pmsg[i].wParam : ' '),
								LOWORD(pmsg[i].lParam), HIWORD(pmsg[i].lParam) & 0xFF,
								0x01000000 & pmsg[i].lParam ? szYes : szNo,
								0x20000000 & pmsg[i].lParam ? szYes : szNo,
								0x40000000 & pmsg[i].lParam ? szDown : szUp,
								0x80000000 & pmsg[i].lParam ? szUp : szDown));


		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}