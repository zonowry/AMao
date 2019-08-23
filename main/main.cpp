#include "SpriteBuilder.h"

int WINAPI WinMain(_In_ HINSTANCE  hInstance, _In_opt_ HINSTANCE prevInstance, _In_ char* lpCmdLine, _In_ int cmdShow)
{
	SpriteBuilder builder;
	builder.Configuration(hInstance);
	builder.Run();
}