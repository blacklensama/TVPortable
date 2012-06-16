#include "main.h"

#include "Classes\AppDelegate.h"

#include <windows.h>
#include <io.h>

#define USE_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
	AppDelegate app;

	// for debug usage
	// as there's no debug console for vs
#ifdef USE_CONSOLE
	AllocConsole(); 
	stdout->_file = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE),0);
#endif

    return cocos2d::CCApplication::sharedApplication().run();
}
