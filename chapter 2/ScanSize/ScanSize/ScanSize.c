/*---------------------------------------------
ScanSize.c
	Displays screen size in a message box 
with variable argument
	code and compile it with visual studio 2008
----------------------------------------------*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int CDECL MessageBoxPrintf(TCHAR* szCaption, TCHAR* szFormat, ...)
{
	TCHAR szBuffer[1024] = {0};
	va_list pArgList;

	va_start(pArgList, szFormat);

	_vsntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), 1024,
				szFormat, pArgList);

	va_end(pArgList);
	return MessageBox(NULL, szBuffer, szCaption, 0);
}

int WINAPI WinMain(HINSTANCE hInstace, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nShowCmd)
{
	int cxScreen = 0, cyScreen = 0;
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	MessageBoxPrintf(TEXT("Screen Size"),
					TEXT("The screen is %d pixels wide by %d pixels high"),
					cxScreen, cyScreen);

	return 0;
}