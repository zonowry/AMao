#include "SpriteBuilder.h"


void SpriteBuilder::Configuration(HINSTANCE  hInstance)
{
	// TODO: 配置文件读取配置，目前写死
	int width = 128, height = 128, x = 500, y = 500;
	SpriteConfiguration config = {
		SIZE{width,height},
		POINT{x,y},
		hInstance
	};
	// 读取精灵配置文件
	SpriteInstance::GetInstance()->Ready(config);

}


SpriteBuilder::SpriteBuilder()
{
}

SpriteBuilder::~SpriteBuilder()
{
}


void SpriteBuilder::Run()
{
	SpriteInstance::GetInstance()->Start();
}

