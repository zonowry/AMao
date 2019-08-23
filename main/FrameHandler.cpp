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

FrameHandler::FrameHandler() :window(NULL), clientRect({ 0 }), size({ 0 })
{
}




void FrameHandler::SetWindowHand(HWND window, SIZE size)
{
	this->window = window;
	this->size = size;
	this->clientRect = { 0,0,size.cx,size.cy };
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
	this->pDCtarget->BindDC(this->hdcSrc, &clientRect);
	// 初始化 WIC 图片工厂
	HR(CoInitialize(NULL));
	HR(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory)));

}


void FrameHandler::NextFrame()
{
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
		_snprintf(buf, sizeof(buf)
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
