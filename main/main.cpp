#include "SpriteBuilder.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

int WINAPI WinMain(_In_ HINSTANCE  hInstance, _In_opt_ HINSTANCE prevInstance, _In_ char* lpCmdLine, _In_ int cmdShow)
{
	SpriteBuilder builder;
	builder.Configuration(hInstance);
	builder.Run();
}