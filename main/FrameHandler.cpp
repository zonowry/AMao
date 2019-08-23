#include "FrameHandler.h"


struct ComException
{
	HRESULT result;
	ComException(HRESULT const value) :result(value)
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();
		LPTSTR errorText = NULL;
		//_com_error error(hresult);
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			value,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)& errorText,  // output 
			0, // minimum size for output buffer
			NULL);   // arguments - see note 

		if (NULL != errorText)
		{
			MessageBox(NULL, errorText, TEXT("ComException"), NULL);
			// release memory allocated by FormatMessage()
			LocalFree(errorText);
			errorText = NULL;
		}
	}
};

void HR(HRESULT const result)
{
	if (S_OK != result)
		throw ComException(result);
}



SpriteFrame::SpriteFrame() :id(SpriteFrame::CeateFrameId()) {

}

FrameHandler::FrameHandler() : window(NULL), rect({ 0 }), size({ 0 })
{
}

void FrameHandler::SetWindowHand(HWND window, SIZE size)
{
	this->window = window;
	this->size = size;
	this->rect = { 0,0,size.cx,size.cy };
	this->d2d1Rect = { 0,0, (float)size.cx, (float)size.cy };
	/// 初始化hdc  创建兼容dc >>>
	this->hdcDst = GetDC(this->window);
	this->hdcSrc = CreateCompatibleDC(this->hdcDst);
	HBITMAP memBitmap = ::CreateCompatibleBitmap(hdcDst, size.cx, size.cy);
	::SelectObject(this->hdcSrc, memBitmap);

	HRESULT hr = NULL;
	// 初始化Direct2D（主要)工厂 
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	HR(pD2DFactory->CreateDCRenderTarget(
		&D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		&pDCtarget));
	// 绑定dc
	this->pDCtarget->BindDC(this->hdcSrc, &this->rect);
	// 初始化 WIC 图片工厂
	HR(CoInitialize(NULL));
	HR(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory)));

}

ID2D1Bitmap* FrameHandler::CreateBitmap(IStream* pStream1)
{
	HRESULT hr = S_OK;
	//创建wic（位图）解码器  
	IWICBitmapDecoder* pDecoder;

	pIWICFactory->CreateDecoderFromStream(pStream1, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
	// 解码后，获取图片第一帧
	IWICBitmapFrameDecode* pFrame;
	pDecoder->GetFrame(0, &pFrame);


	// 创建图片格式化转换器
	IWICFormatConverter* pConverter;
	pIWICFactory->CreateFormatConverter(&pConverter);

	pConverter->Initialize(
		pFrame,                          // 位图数据
		GUID_WICPixelFormat32bppPBGRA,   // 转换的像素格式 
		WICBitmapDitherTypeNone,         // Specified dither pattern  
		NULL,                            // Specify a particular palette   
		0.f,                             // Alpha threshold  
		WICBitmapPaletteTypeCustom       // Palette translation type  
	);
	ID2D1Bitmap* pBitmap = NULL;
	pDCtarget->CreateBitmapFromWicBitmap(pConverter, nullptr, &pBitmap);
	return pBitmap;
}

ID2D1Bitmap* FrameHandler::mapFrameToImage(SpriteFrame* frame)
{
	if (frameImageStorge[frame->id] == NULL) {
		frameImageStorge[frame->id] = CreateBitmap(frame->ToStream());
	}
	return frameImageStorge[frame->id];
}

void FrameHandler::NextFrame(SpriteFrame* frame)
{
	// 绘制帧相同，无需重绘
	if (this->currentFrameId == frame->id) {
		return;
	}
	pDCtarget->BeginDraw();
	pDCtarget->Clear();
	pDCtarget->DrawBitmap(mapFrameToImage(frame), this->d2d1Rect);
	pDCtarget->EndDraw();
	this->currentFrameId = frame->id;

	POINT ptSrc = { 0 };
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;
	::UpdateLayeredWindow(this->window, this->hdcDst, NULL, &this->size, this->hdcSrc, &ptSrc, NULL, &bf, ULW_ALPHA);
}

string SpriteFrame::CeateFrameId()
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

SpriteFrameResource::SpriteFrameResource(FrameResource resourceId) :resourceId(resourceId)
{
}

IStream* SpriteFrameResource::ToStream()
{
	HRSRC hRes = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(FRAME_TEST), L"PNG");
	HGLOBAL  hMem = ::LoadResource(NULL, hRes);
	IStream* pStream1 ;
	HR(CreateStreamOnHGlobal(hMem, TRUE, &pStream1));
	return pStream1;
}

SpriteFrameFile::SpriteFrameFile(string path) :filePath(path)
{
}

IStream* SpriteFrameFile::ToStream()
{
	FILE* file;
	fopen_s(&file, this->filePath.c_str(), "rb");
	IStream* s;
	fread_s(&s, 100, 0, 100, file);

	return s;
}
