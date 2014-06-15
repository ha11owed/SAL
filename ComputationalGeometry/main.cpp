#include "preheader.h"
#include "renderer.h"
#include "demos.h"

int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
{
	DemoScene demo;
	Renderer& renderer = Renderer::instance();
	renderer.scene = &demo;
	renderer.mainLoop();

#ifdef _MSC_VER
	/* DUMP MEMORY LEAK INFORMATION */
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}