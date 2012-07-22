#include "main.h"

#include <windows.h>
#include <io.h>
#include "TVPortable/Visual/Application.h"

#define USE_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
 	TVPortable::Visual::Application& myApp = TVPortable::Visual::Application::App(TVPortable::Visual::ApplicationConfig::DefaultConfig());


	// for debug usage
	// as there's no debug console for vs
#ifdef USE_CONSOLE
	AllocConsole(); 
	stdout->_file = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE),0);
#endif

    myApp.run();

    return 0;
}
