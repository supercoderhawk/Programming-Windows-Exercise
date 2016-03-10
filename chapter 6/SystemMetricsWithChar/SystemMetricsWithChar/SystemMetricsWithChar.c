/*--------------------------------------------------
SystemMetricsWithChar.c
	Display system metrics in a window with a 
horizontal and vertical scroll bar.
	Window can accept char message, too.
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

	HDC hdc = NULL;
	PAINTSTRUCT ps;
	static int cxChar = 0, cyChar = 0, cxCaps = 0, cyCaps = 0;
	static int cxClient = 0, cyClient = 0, iMaxWidth = 0;
	TEXTMETRIC tm;
	int iLength = 0;
	TCHAR szBuffer[64] = {0};
	int i = 0, x = 0, y = 0, iVertPos = 0, iHorzPos = 0;
	int iPaintBegin = 0, iPaintEnd = 0;
	SCROLLINFO si;
	
	switch(message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily&1 ?3:2)*cxChar/2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		iMaxWidth = 40 * cxChar + 22 * cxCaps;
		ReleaseDC(hWnd, hdc);
		break;
	case WM_PAINT:
		hdc= BeginPaint(hWnd, &ps);
		
		//get vertical scroll bar position
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd, SB_VERT, &si);
		iVertPos = si.nPos;

		//get horizontal scroll bar position
		GetScrollInfo(hWnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		//find painting limits
		iPaintBegin = max(0, iVertPos + ps.rcPaint.top / cyChar);
		iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

		for(i = iPaintBegin; i < iPaintEnd; i++)
		{
			x = cxChar * (1 - iHorzPos);
			y = cyChar * (i - iVertPos);

			TextOut(hdc, x, y, sysmetrics[i].szLabel,
					lstrlen(sysmetrics[i].szLabel)); 

			TextOut(hdc, x + 22 * cxCaps , y, sysmetrics[i].szDesc,
					lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_TOP | TA_RIGHT);

			TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, 
				swprintf_s(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), TEXT("%5d"), sysmetrics[i].iIndex));

			SetTextAlign(hdc, TA_TOP | TA_LEFT);
		}
		
		EndPaint(hWnd, &ps);
		break;
	case WM_HSCROLL:
		//get all the horizontal scroll bar information
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		
		//save the position for comparison later on
		GetScrollInfo(hWnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		switch(LOWORD(wParam))
		{
		case SB_LINELEFT:
			si.nPos -= 1;
			break;
		case SB_LINERIGHT:
			si.nPos += 1;
			break;
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;
		case SB_THUMBPOSITION:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}

		//set the position and then retrieve it. Due to adjustments
		//by Windows it may not the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hWnd, SB_HORZ, &si);

		//if position has changed, scroll the window
		if(si.nPos != iHorzPos)
		{
			ScrollWindow(hWnd, cxChar * (iHorzPos - si.nPos), 0,
						NULL, NULL);
		}
		break;
	case WM_VSCROLL:
		//get all scroll bar information
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);

		//save position for comparison later on

		iVertPos = si.nPos;

		switch(LOWORD(wParam))
		{
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}

		//set the position and retrieve it. Due to adjustments by Windows
		//it may not be the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);

		//check the position, if changed, move the scrollbar.
		if(si.nPos != iVertPos)
		{
			ScrollWindow(hWnd, 0, cyChar*(iVertPos - si.nPos), NULL, NULL);
			UpdateWindow(hWnd);
		}

		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		//set vertical scroll bar range and page size
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = NUMLINES - 1;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		//set horizontal scroll bar range and page size
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + iMaxWidth / cxChar;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

		break;
	
	case WM_CLOSE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}