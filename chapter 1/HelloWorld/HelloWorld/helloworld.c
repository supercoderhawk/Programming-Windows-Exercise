/*---------------------------------------------
helloworld.c 
	Displays  "Hello, World." in a messagebox
	code and compile it with visual studio 2008
----------------------------------------------*/

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nShowCmd)
{

	MessageBox(NULL, TEXT("Hello World."), TEXT("HelloMsg"), MB_YESNOCANCEL|MB_DEFBUTTON1|MB_ICONASTERISK);
	
	return 0;
}