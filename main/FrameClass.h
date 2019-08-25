
#pragma once
#include <Windows.h>
#include "resource.h"
#include <fstream>
#include <wincodec.h>


using namespace std;

class SpriteFrame {
protected:
	static char* CeateFrameId();
	IWICImagingFactory* pIWICFactory;

public:
	SpriteFrame();
	const char* id;
	void SetWICFactory(IWICImagingFactory* pIWICFactory);
	virtual IStream* ToStream() = 0;
};


class SpriteFrameResource : public SpriteFrame {
private:
	const UINT resourceId;
public:
	SpriteFrameResource(UINT resourceId);
	IStream* ToStream() override;
};


class SpriteFrameFile : public SpriteFrame {
private:
	const LPCWSTR filePath;
public:
	SpriteFrameFile(LPCWSTR);
	IStream* ToStream() override;
};
