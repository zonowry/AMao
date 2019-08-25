#include "FrameClass.h"

SpriteFrame::SpriteFrame() :id(SpriteFrame::CeateFrameId()) {

}

void SpriteFrame::SetWICFactory(IWICImagingFactory* pIWICFactory)
{
	this->pIWICFactory = pIWICFactory;
}

char* SpriteFrame::CeateFrameId()
{
	static char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		snprintf(buf, sizeof(buf)
						 , "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
						 , guid.Data1
						 , guid.Data2
						 , guid.Data3
						 , guid.Data4[0], guid.Data4[1]
						 , guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
						 , guid.Data4[6], guid.Data4[7]
		);
	}
	return buf;
}

/////////////////////////
//	  Frame Resource   //
/////////////////////////
SpriteFrameResource::SpriteFrameResource(UINT resourceId) :resourceId(resourceId)
{
}

IStream* SpriteFrameResource::ToStream()
{
	// 获取资源
	HRSRC hPic = ::FindResource(NULL, MAKEINTRESOURCE(FRAME_TEST), FRAME_SPRITE);
	HANDLE hResData = NULL;
	if (!hPic || !(hResData = LoadResource(NULL, hPic)))
	{
		return NULL;
	}
	HGLOBAL pImageFile = LockResource(hResData);
	// Calculate the size.
	long size = SizeofResource(NULL, hPic);
	IWICStream* pStream = NULL;
	this->pIWICFactory->CreateStream(&pStream);
	pStream->InitializeFromMemory(
		reinterpret_cast<BYTE*>(pImageFile),
		size
	);
	return pStream;
}


/////////////////////////
//	   Frame File      //
/////////////////////////
SpriteFrameFile::SpriteFrameFile(LPCWSTR path) : filePath(path)
{
}


//IStream* SpriteFrameFile::ToStream()
//{
//	// 1. 打开图片文件
//	ifstream is(this->filePath, ifstream::in);
//	// 2. 计算图片长度
//	is.seekg(0, is.end);
//	int length = is.tellg();
//	is.seekg(0, is.beg);
//	// 3. 创建内存缓存区
//	IStream* buffer = new IStream ();
//	// 4. 读取图片
//	is.read(buffer, length);
//	// 到此，图片已经成功的被读取到内存（buffer）中
//	return  (IStream * )buffer;
//}
IStream* SpriteFrameFile::ToStream()
{
	IWICStream* pStream = NULL;
	this->pIWICFactory->CreateStream(&pStream);
	pStream->InitializeFromFilename(this->filePath, GENERIC_READ);
	return pStream;
}
