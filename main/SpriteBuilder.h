#pragma once
#include <Windows.h>
#include "SpriteInstance.h"



// 精灵构造器，程序初始化
class SpriteBuilder {
private:

public:
	SpriteBuilder();
	~SpriteBuilder();

	void Configuration(HINSTANCE  hInstance);

	void Run();
};