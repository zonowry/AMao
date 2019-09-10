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

void FrameHandler::NextFrame(SpriteFrame* frame)
{
	// 绘制帧相同，无需重绘
	if (this->currentFrameId == frame->id) {
		return;
	}
	ID2D1Bitmap* img = mapFrameToImage(frame);
	if (img == NULL) {
		return;
	}
	pDCtarget->BeginDraw();
	pDCtarget->Clear();
	pDCtarget->DrawBitmap(img, this->d2d1Rect);
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

ID2D1Bitmap* FrameHandler::mapFrameToImage(SpriteFrame* frame)
{
	if (frameImageStorge[frame->id] == NULL) {
		frame->SetWICFactory(this->pIWICFactory);
		frameImageStorge[frame->id] = CreateBitmap(frame->ToStream());
	}
	return frameImageStorge[frame->id];
}
