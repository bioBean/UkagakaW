
#pragma region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018 Bio-Electric Studio
* All Right Reserved
* This C++ source file is for the Kiwi Renderer, which is part of the Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* Author: Gray_Neko_Bean
*
* Overall Description:
* None
*
* Update Description:
* None
*
*
* ---------------------------------------------------
*/

#pragma endregion

#pragma once

#include "stdafx.h"

#include "UWRenderer.h"

extern RenderEvent MT_OnGeneralRender;
extern RenderEvent MT_OnAnimFinishPlay;

extern UINT animationFrameRate;

UWD2DRenderer::UWD2DRenderer(HWND hWnd) : mFactory(NULL),
										  mDCRT(NULL),
										  MainHwnd(hWnd),
										  mGdiRT(NULL),
										  mWicImgFactory(NULL),
										  mDwFactory(NULL)
{
	InitRenderer(hWnd);
}

HRESULT UWD2DRenderer::InitRenderer(HWND hwnd)
{
	HRESULT hr;

	MainHwnd = hwnd;
	RECT WndRect;
	GetWindowRect(hwnd, &WndRect);

	WndPos = {
		WndRect.top,
		WndRect.left};

	WndSize = {
		WndRect.right - WndRect.left,
		WndRect.bottom - WndRect.top};

	hr = this->CreateDIR();
	hr = this->CreateDDR();

	return hr;
}

HRESULT UWD2DRenderer::CreateDIR()
{
	HRESULT hr;

	hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->mFactory);

	if (this->mDwFactory == NULL && SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(this->mDwFactory),
			reinterpret_cast<IUnknown **>(&this->mDwFactory));
	}

	if (!this->mWicImgFactory && SUCCEEDED(hr))
	{
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&this->mWicImgFactory));
	}

	return hr;
}

HRESULT UWD2DRenderer::CreateDDR()
{
	HRESULT hr = S_OK;

	if (mFactory != NULL && this->mDCRT == NULL)
	{
		D2D1_RENDER_TARGET_PROPERTIES rtp = RenderTargetProperties();
		rtp.usage = D2D1_RENDER_TARGET_USAGE::D2D1_RENDER_TARGET_USAGE_NONE;
		rtp.type = D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_HARDWARE;
		rtp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM,
											D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED);
		rtp.minLevel = D2D1_FEATURE_LEVEL::D2D1_FEATURE_LEVEL_DEFAULT;

		hr = mFactory->CreateDCRenderTarget(&rtp, &this->mDCRT);

		UWDefinedBrushResources[UWTextColor::Black] = NULL;
		D2D1_COLOR_F brushColor;
		brushColor.a = 255;
		brushColor.r = 0;
		brushColor.g = 0;
		brushColor.b = 0;

		hr = mDCRT->CreateSolidColorBrush(brushColor, &UWDefinedBrushResources[UWTextColor::Black]);
		
		IDWriteFontCollection *fontCollection;
		mDwFactory->GetSystemFontCollection(&fontCollection, FALSE);

		this->CreateFontStyle(L"Consolas", 10.0f, false, false);
		this->CreateFontStyle(L"Consolas", 10.0f, true, false);
		this->CreateFontStyle(L"Consolas", 10.0f, false, true);
		this->CreateFontStyle(L"Consolas", 7.0f, false, false);
		this->CreateFontStyle(L"Consolas", 12.5f, false, false);
		this->CreateFontStyle(L"Consolas", 15.0f, false, false);

		//TextStyleResources[UWTextStyle::paragraph] = NULL;
		//mDwFactory->CreateTextFormat(L"Consolas", fontCollection,
		//	DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		//	DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, 10.0f, L"cn-zh", &TextStyleResources[UWTextStyle::paragraph]);

		//TextStyleResources[UWTextStyle::strong] = NULL;
		//mDwFactory->CreateTextFormat(L"Consolas", fontCollection,
		//	DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		//	DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, 10.0f, L"cn-zh", &TextStyleResources[UWTextStyle::strong]);

		//TextStyleResources[UWTextStyle::italic] = NULL;
		//mDwFactory->CreateTextFormat(L"Consolas", fontCollection,
		//	DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC,
		//	DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, 10.0f, L"cn-zh", &TextStyleResources[UWTextStyle::italic]);

		//TextStyleResources[UWTextStyle::small] = NULL;
		//mDwFactory->CreateTextFormat(L"Consolas", fontCollection,
		//	DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		//	DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, 7.0f, L"cn-zh", &TextStyleResources[UWTextStyle::small]);

		//TextStyleResources[UWTextStyle::big] = NULL;
		//mDwFactory->CreateTextFormat(L"Consolas", fontCollection,
		//	DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		//	DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, 12.5f, L"cn-zh", &TextStyleResources[UWTextStyle::big]);

		//TextStyleResources[UWTextStyle::large] = NULL;
		//mDwFactory->CreateTextFormat(L"Consolas", fontCollection,
		//	DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		//	DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL, 15.0f, L"cn-zh", &TextStyleResources[UWTextStyle::large]);

		for(UWTextFont font : UWDefinedFonts)
		{
			TextStyleResources.push_back(NULL);

			mDwFactory->CreateTextFormat(font.font, fontCollection,
				(!font.bold) ? DWRITE_FONT_WEIGHT_NORMAL : DWRITE_FONT_WEIGHT_BOLD,
				(!font.italic) ? DWRITE_FONT_STYLE_NORMAL : DWRITE_FONT_STYLE_ITALIC,
				DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
				font.size, L"cn-zh", &TextStyleResources[TextStyleResources.size() - 1]);
		}

		this->LoadResources(L".\\Resources");
	}

	return hr;
}

int UWD2DRenderer::CreateFontStyle(LPCWSTR fontFamily, float size, bool bold, bool italic) {
	UWTextFont font = UWTextFont();
	font.font = fontFamily;
	font.size = size;
	font.bold = bold;
	font.italic = italic;

	this->UWDefinedFonts.push_back(font);

	CComPtr<IDWriteFontCollection> fontCollection;
	mDwFactory->GetSystemFontCollection(&fontCollection, FALSE);

	TextStyleResources.push_back(NULL);

	mDwFactory->CreateTextFormat(font.font, fontCollection,
		(!font.bold) ? DWRITE_FONT_WEIGHT_NORMAL : DWRITE_FONT_WEIGHT_BOLD,
		(!font.italic) ? DWRITE_FONT_STYLE_NORMAL : DWRITE_FONT_STYLE_ITALIC,
		DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
		font.size, L"cn-zh", &TextStyleResources[TextStyleResources.size() - 1]);

	return UWDefinedFonts.size() - 1;
}

int UWD2DRenderer::CreateColorBrush(int r, int g, int b, int a) {

	D2D1_COLOR_F brushColor = { r, g, b, a };
	int colorCode = CalculateColorCode(r, g, b, a);

	if (UWDefinedBrushResources.count(colorCode) == 0) {
		UWDefinedBrushResources[colorCode];
		 mDCRT->CreateSolidColorBrush(brushColor, &UWDefinedBrushResources[colorCode]);
	}

	return colorCode;
}


HRESULT UWD2DRenderer::DiscardDIR()
{
	this->mDwFactory.Release();
	this->mFactory.Release();
	this->mWicImgFactory.Release();
	//IDWriteFactory* idwf = this->mDwFactory.Detach();
	//ID2D1Factory* idf = this->mFactory.Detach();
	//IWICImagingFactory* iwicif = this->mWicImgFactory.Detach();
	//SafeRelease(&idwf);
	//SafeRelease(&idf);
	//SafeRelease(&iwicif);

	return S_OK;
}

HRESULT UWD2DRenderer::DiscardDDR()
{
	HRESULT hr;

	this->mDCRT.Release();
	for (CPBitmap b : UWBitmapResrouces)
	{
		b.Release();
	}

	return S_OK;
}

HRESULT UWD2DRenderer::CleanUp()
{
	this->mDwFactory.Release();
	this->mFactory.Release();
	this->mWicImgFactory.Release();

	this->mDCRT.Release();

	if (mGdiRT != NULL)
		this->mGdiRT.Release();

	for (CPBitmap b : UWBitmapResrouces)
	{
		if (b != NULL)
		{
			b.Release();
		}
	}

	return S_OK;
}

HRESULT UWD2DRenderer::LoadResources(wstring path)
{

	HRESULT hr = S_OK;
	wstring BitmapPath = path + L"\\Bitmap";
	wstring AnimationPath = path + L"\\Animation";

	vector<wstring> bitmapFiles = GetAllFileNamesWithExtName(BitmapPath, L"png");
	for (wstring bp : bitmapFiles)
	{
		if (SUCCEEDED(hr))
		{
			hr = LoadD2DBitmap(BitmapPath + (L"\\") + (bp), WString2String(bp).substr(0, bp.size() - 4), WndSize.cy);
		}
		else
		{
			return hr;
		}
	}

	vector<wstring> animFiles = GetAllFileNamesWithExtName(AnimationPath, L"gif");
	for (wstring bp : animFiles)
	{
		if (SUCCEEDED(hr))
		{
			hr = LoadD2DAnimation(AnimationPath + (L"\\") + (bp), WString2String(bp).substr(0, bp.size() - 4));
		}
		else
		{
			return hr;
		}
	}

	return hr;
}

void UWD2DRenderer::OnLeftDown(POINT localPos)
{
	POINT gp = GetMouseGlobalPos();

	this->TempPos = WndPos;
	this->MouseStartPos = gp;
}

void UWD2DRenderer::OnLeftDrag_Global(POINT globalPos)
{
	WndPos =
		{
			TempPos.x + (globalPos.x - MouseStartPos.x),
			TempPos.y + (globalPos.y - MouseStartPos.y)};
}

HRESULT UWD2DRenderer::BindDC2RenderTarget(HDC hDC)
{

	HRESULT hr = S_OK;

	RECT rect;
	GetClientRect(MainHwnd, &rect);

	if (SUCCEEDED(hr))
	{
		if (mDCRT != NULL)
		{
			hr = mDCRT->BindDC(hDC, &rect);
		}
		else
		{
			hr = E_FAIL;
		}
	}

	return hr;
}

int UWD2DRenderer::AddNewBitmap(string tag)
{
	int ID = UWBitmapResrouces.size();
	UWBitmapResrouces.push_back(NULL);
	BitmapMappingTable[tag] = ID;

	return ID;
}

CPBitmap UWD2DRenderer::GetBitmapByTag(string tag)
{
	return UWBitmapResrouces[BitmapMappingTable[tag]];
}

HRESULT UWD2DRenderer::LoadD2DBitmap(wstring path, string tag, UINT height, UINT width)
{
	this->mWicImgFactory;

	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	HRESULT hr;

	hr = mWicImgFactory->CreateDecoderFromFilename(
		path.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

	if (SUCCEEDED(hr))
		pDecoder->GetFrame(0, &pSource);
	if (SUCCEEDED(hr))
		hr = mWicImgFactory->CreateFormatConverter(&pConverter);

	if (SUCCEEDED(hr))
	{
		if (width != 0 || height != 0)
		{
			UINT originWidth, originHeight;
			hr = pSource->GetSize(&originWidth, &originHeight);

			float bitmapScale = 1.0f * originHeight / originWidth;

			if (SUCCEEDED(hr))
			{
				if (width == 0 && height != 0)
				{
					//FLOAT scale = static_cast<FLOAT>(height) / static_cast<FLOAT>(originHeight);
					//width = static_cast<UINT>(static_cast<float>(originWidth) * scale);
					float scale = (float)(1.0f * height / originHeight);
					width = (UINT)((float)1.0f * originWidth * scale);
				}
				else if (height == 0 && width != 0)
				{
					float scale = float(1.0f * width / originWidth);
					height = (UINT)((float)1.0f * originHeight * scale);
				}

				hr = mWicImgFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(pSource,
											 width,
											 height,
											 WICBitmapInterpolationMode::WICBitmapInterpolationModeCubic);
				}
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(pScaler,
												GUID_WICPixelFormat32bppPBGRA,
												WICBitmapDitherType::WICBitmapDitherTypeNone,
												NULL,
												0.0F,
												WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut);
				}
			}
		}
		else
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherType::WICBitmapDitherTypeNone,
				NULL,
				0.0F,
				WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut);
		}

		if (SUCCEEDED(hr))
		{
			int bmid = AddNewBitmap(tag);
			hr = this->mDCRT->CreateBitmapFromWicBitmap(pConverter, NULL, &(UWBitmapResrouces[bmid]));

			UWAnimationResources[tag] = SPAnimation(new Animation(bmid));
		}
	}
	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);
	return hr;
}

HRESULT UWD2DRenderer::LoadD2DAnimation(wstring path, string tag, UINT height, UINT width)
{

	IWICImagingFactory *pWicFac = this->mWicImgFactory;

	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICMetadataQueryReader *pMetaReader = NULL;
	IWICBitmapScaler *pScaler = NULL;

	HRESULT hr;

	wstring extName = path.substr(path.length() - 4, 4);

	TIMELINE_SEC tl = TIMELINE_SEC();
	vector<int> anim = vector<int>();

	if (extName._Equal(L".gif"))
	{

		hr = pWicFac->CreateDecoderFromFilename(
			path.c_str(), NULL, GENERIC_READ, WICDecodeOptions::WICDecodeMetadataCacheOnLoad, &pDecoder);

		UINT frameCount;
		hr = pDecoder->GetFrameCount(&frameCount);

		PROPVARIANT meta;
		PropVariantInit(&meta);

		for (int f = 0; f < frameCount; f++)
		{

			int frameDelay = 40;

			if (SUCCEEDED(hr))
			{
				hr = pDecoder->GetFrame(f, &pSource);
			}

			if (SUCCEEDED(hr))
			{
				hr = pSource->GetMetadataQueryReader(&pMetaReader);
				if (SUCCEEDED(hr))
				{
					hr = pMetaReader->GetMetadataByName(L"/grctlext/Delay", &meta);
					hr = (meta.vt == VT_UI2 ? S_OK : E_FAIL);
					if (SUCCEEDED(hr))
					{
						frameDelay = meta.uiVal * 10;
					}
				}
			}

			if (SUCCEEDED(hr))
			{
				hr = pWicFac->CreateFormatConverter(&pConverter);
			}

			if (SUCCEEDED(hr))
			{
				if (width != 0 || height != 0)
				{
					UINT originWidth, originHeight;
					hr = pSource->GetSize(&originWidth, &originHeight);

					if (SUCCEEDED(hr))
					{
						if (width == 0)
						{
							//FLOAT scale = static_cast<FLOAT>(height) / static_cast<FLOAT>(originHeight);
							//width = static_cast<UINT>(static_cast<float>(originWidth) * scale);
							float scale = (float)(width / originWidth);
							width = (UINT)((float)originWidth * scale);
						}
						if (height == 0)
						{
							float scale = float(width / originWidth);
							height = (UINT)((float)originHeight * scale);
						}

						hr = pWicFac->CreateBitmapScaler(&pScaler);
						if (SUCCEEDED(hr))
						{
							hr = pScaler->Initialize(pSource,
													 width,
													 height,
													 WICBitmapInterpolationMode::WICBitmapInterpolationModeCubic);
						}
						if (SUCCEEDED(hr))
						{
							hr = pConverter->Initialize(pScaler,
														GUID_WICPixelFormat32bppPBGRA,
														WICBitmapDitherType::WICBitmapDitherTypeNone,
														NULL,
														0.0F,
														WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut);
						}
					}
				}
				else
				{
					hr = pConverter->Initialize(
						pSource,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherType::WICBitmapDitherTypeNone,
						NULL,
						0.0F,
						WICBitmapPaletteType::WICBitmapPaletteTypeMedianCut);
				}
			}

			if (SUCCEEDED(hr))
			{
				string tg = tag + "_" + std::to_string(f);
				tl.push_back(pair<Milliseconds, string>(frameDelay, tg));
				int bmp = AddNewBitmap(tg);
				anim.push_back(bmp);
				hr = this->mDCRT->CreateBitmapFromWicBitmap(pConverter, NULL, &UWBitmapResrouces[bmp]);
			}
		}
		UWAnimationResources[tag] = SPAnimation(new Animation(BitmapMappingTable, tl, (int)(1000 / animationFrameRate)));
		UWAnimationResources[tag]->currentRenderFrameRate = animationFrameRate;
	}
	else if (extName == L".anm")
	{
		//TODO: read .anm bitmap anim description file
	}

	return hr;
}

HRESULT UWD2DRenderer::CreateDCRTEnv(POINT position, SIZE size)
{

	int ix, iy;

	ix = size.cx;
	iy = size.cy;

	DCRTENV_SCREENDC = GetDC(NULL);
	DCRTENV_MEMDC = CreateCompatibleDC(DCRTENV_SCREENDC);

	BYTE *pBits;
	BITMAPINFOHEADER bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = ix;
	bmih.biHeight = iy;
	bmih.biPlanes = 1;
	bmih.biBitCount = 32; //����һ��Ҫ��32 16λɫû��alphaͨ��
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	DCRTENV_CANVAS = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID **)&pBits, NULL, 0);

	SelectObject(DCRTENV_MEMDC, DCRTENV_CANVAS);

	HRESULT hr = BindDC2RenderTarget(DCRTENV_MEMDC);

	return hr;
}

void UWD2DRenderer::ReleaseDCRTEnv()
{

	DeleteDC(DCRTENV_MEMDC);
	DeleteDC(DCRTENV_SCREENDC);
	DeleteObject(DCRTENV_CANVAS);

	ReleaseDC(MainHwnd, DCRTENV_MEMDC);
	ReleaseDC(NULL, DCRTENV_SCREENDC);
}

HRESULT UWD2DRenderer::OnDrawFrame(Milliseconds &deltaTime)
{

	Timer timer = Timer();
	timer.Start();

	HRESULT hr;

	POINT position = WndPos;
	SIZE size = WndSize;

	hr = CreateDCRTEnv(position, size);

	HDC screenDC = DCRTENV_SCREENDC,
		memDC = DCRTENV_MEMDC;
	HBITMAP canvas = DCRTENV_CANVAS;

	if (SUCCEEDED(hr))
	{

		try
		{
			mDCRT->BeginDraw();

			Milliseconds dt;
			OnFrameRender(this->PopBuffer(), position, size, dt);
			//CPBitmap bm = GetBitmapByTag("BM0_Test");
			//mDCRT->DrawBitmap(bm, { 0, 0, 100, 100 }, 1.0f);

			mDCRT->EndDraw();
		}
		catch (_com_error &e)
		{
			IErrorInfo *info = e.ErrorInfo();
			BSTR errDesc;
			BSTR errSource;
			hr = info->GetDescription(&errDesc);
			hr = info->GetSource(&errSource);

			_bstr_t err = e.Description();
			Error(LPCWSTR(err));

			info->Release();
		}
	}
	deltaTime = timer.GetMilliseconds();
	timer.End();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDDR();
		CreateDDR();
	}
	InvalidateRect(MainHwnd, NULL, FALSE);

	return hr;
}

void UWD2DRenderer::RenderOnScreen()
{

	HRESULT hr = S_OK;

	POINT pPos = WndPos;
	POINT pSrc = {0, 0};
	SIZE sizeWnd = WndSize;

	BLENDFUNCTION blendFunc;
	blendFunc.AlphaFormat = AC_SRC_ALPHA;
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 255;
	//SetLayeredWindowAttributes(hwnd, 0, 255, ULW_ALPHA);
	bool re = UpdateLayeredWindow(MainHwnd, DCRTENV_SCREENDC, &pPos, &sizeWnd, DCRTENV_MEMDC, &pSrc, NULL, &blendFunc, ULW_ALPHA);
	DWORD err = GetLastError();

	ReleaseDCRTEnv();
}

HRESULT UWD2DRenderer::OnFrameRender(RenderTask task, POINT position, SIZE canvasSize, _Out_ Milliseconds &deltaTime)
{
	Timer timer = Timer();
	timer.Start();
	if (!task.IsEmpty())
	{
		task.Render(mDCRT, mDwFactory, position, canvasSize);
		mLastBuffer = task;
	}
	else
	{
		mLastBuffer.Render(mDCRT, mDwFactory, position, canvasSize);
	}

	deltaTime = timer.GetMilliseconds();
	timer.End();

	return S_OK;
}

RenderTask UWD2DRenderer::PopBuffer()
{
	if (mBuffer.size() > 0)
	{
		RenderTask task = mBuffer.front();
		mBuffer.pop();
		return task;
	}
	else
	{
		Warning(L"Buffer pool empty, frame render stroked, please check any time-cost codes in Main Thread");
		return RenderTask();
	}
}

BOOL UWD2DRenderer::FetchBuffer(RenderTask renderTask)
{
	if (mBuffer.size() < 1)
	{
		this->mBuffer.push(renderTask);
		return true;
	}
	else
	{
		return false;
	}
}

BOOL UWD2DRenderer::IsBufferEmpty()
{
	return mBuffer.empty();
}

HRESULT UWRenderElement_Bitmap::Render(CPDCRenderTarget &DCRT, CPDWriteFactory &DWFactory)
{
	DCRT->DrawBitmap(bitmap, rect, opaque);
	return S_OK;
}

VOID UWRenderElement_Text::AppendText(wstring txt, UWTextFont style) {
	text.append(txt);

	DWRITE_TEXT_RANGE range = { text.size(), txt.size() };

	formats.push_back({ range, style });
}

HRESULT UWRenderElement_Text::Render(CPDCRenderTarget &DCRT, CPDWriteFactory &DWFactory)
{
	int textLength = text.size();

	CComPtr<IDWriteTextLayout> layout;
	DWFactory->CreateTextLayout(text.c_str(), textLength, textFormat, rect.right - rect.left, rect.bottom - rect.top, &layout);

	for (auto format : formats) {
		int start = format.range.startPosition;
		int end = start + format.range.length;
		if (start <= textLength) {
			DWRITE_TEXT_RANGE rng;
			if (end > textLength) {
				rng = { (UINT32)start, (UINT32)(textLength - start) };
				layout->SetFontStyle(format.font.GetStyle(), rng);
				layout->SetFontWeight(format.font.GetWeight(), rng);
				layout->SetFontStretch(format.font.GetStretch(), rng);
				layout->SetFontSize(format.font.size, rng);
				layout->SetFontFamilyName(format.font.font, rng);
			}
			else {
				rng = format.range;
				layout->SetFontStyle(format.font.GetStyle(), rng);
				layout->SetFontWeight(format.font.GetWeight(), rng);
				layout->SetFontStretch(format.font.GetStretch(), rng);
				layout->SetFontSize(format.font.size, rng);
				layout->SetFontFamilyName(format.font.font, rng);
			}
		}
	}

	DCRT->DrawTextLayout({ rect.left, rect.top }, layout, textBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

	//DCRT->DrawText(text.c_str(), text.size(), textFormat, rect, textBrush,
	//			   D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT,
	//			   DWRITE_MEASURING_MODE::DWRITE_MEASURING_MODE_NATURAL);
	

	return S_OK;
}

//
//UkagakaTask::UkagakaTask(wstring text)
//{
//	// size_t len = lstrlenW(text.c_str());
//	// Parameters = malloc(len * sizeof(WCHAR) + 1);
//	// wcscpy_s(static_cast<LPWSTR>(Parameters), len * sizeof(WCHAR), text.c_str());
//	this->strParameter = text;
//}

//UkagakaTask::UkagakaTask(UkagakaTaskType type, void *parameter)
//{
//	TaskType = type;
//	// switch (type) {
//	// case UkagakaTaskType::TextOutput:
//	// {
//	// 	size_t size = lstrlenW(static_cast<LPCWSTR>(parameter)) * sizeof(WCHAR);
//	// 	Parameters = malloc(size);
//	// 	wcscpy_s(static_cast<LPWSTR>(Parameters), size, static_cast<LPCWSTR>(parameter));
//	// 	break;
//	// }
//	// case UkagakaTaskType::Wait:
//	// {
//	// 	Parameters = parameter;
//	// 	break;
//	// }
//	// }
//	switch (type)
//	{
//	case UkagakaTaskType::TextOutput:
//	{
//		this->strParameter = *((wstring *)parameter);
//		break;
//	}
//	case UkagakaTaskType::Wait:
//	{
//		this->intParameter = *((int *)parameter);
//		break;
//	}
//	}
//}

//wstring UkagakaTask::GetString()
//{
//	// LPCWSTR str = static_cast<LPCWSTR>(Parameters);
//	// return wstring(str);
//
//	return this->strParameter;
//}
//
//SPAnimation UkagakaTask::GetAnimation()
//{
//	// return *((SPAnimation*)Parameters);
//	return this->animationParameter;
//}
//
//int UkagakaTask::GetInteger()
//{
//	// return *((int*)Parameters);
//	return this->intParameter;
//}

HRESULT UkagakaRenderer::PlayAnimation(string id, UWAnimationState state)
{
	changing = true;

	currentAnimation = pDirect2DRenderer->UWAnimationResources[id];
	nextAnimState = state;

	return S_OK;
}

HRESULT UkagakaRenderer::PlayAnimationImmediately(string id, UWAnimationState state)
{
	bitmapQueue = queue<AnimFrame>();
	return PlayAnimation(id, state);
}

HRESULT UkagakaRenderer::PlayAnimationTB(string uid, UWAnimationState state, bool immediately){
	PushTask(UkagakaTaskAnimation(uid, state, immediately));
	return true;
}

HRESULT UkagakaRenderer::OutputTextTB(wstring text, int style, int color)
{
	this->ShowBalloon = true;
	//this->taskBuffer.push(text);

	UkagakaTaskTextOutput task = UkagakaTaskTextOutput(text, this->pDirect2DRenderer->UWDefinedFonts[style], color);

	this->PushTask(task);
	return S_OK;
}

HRESULT UkagakaRenderer::WaitTicksTB(int ticks)
{
	if (ticks > 0)
	{
		PushTask(UkagakaTaskWait(ticks));
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT UkagakaRenderer::NewPhaseTB() {
	PushTask(UkagakaTaskNewPhase());
	return S_OK;
}

HRESULT UkagakaRenderer::EndSectionTB() {
	PushTask(UkagakaTaskEnd());
	return S_OK;
}

HRESULT UkagakaRenderer::PushTask(UkagakaTask task){
	UkagakaTask t = task;

	if (this->currentStatusEnd)
	{
		if(t.TaskType != -1){
			currentStatusEnd = false;

			this->ClearText();
			this->ClearBuffer();
			this->HideBalloon();
		}
	}

	taskBuffer.push(t);

	return S_OK;
}

HRESULT UkagakaRenderer::ClearText()
{
	this->cacheBalloonText = L"";
	this->currentBalloonText = L"";

	this->currentBalloonTextIndex = 0;

	return S_OK;
}

UkagakaTask::operator UkagakaTaskAnimation()
{
	return UkagakaTaskAnimation(strPar, (UWAnimationState)intPar, boolPar);
}

UkagakaTask::operator UkagakaTaskTextOutput()
{
	UkagakaTaskTextOutput task = UkagakaTaskTextOutput(wstrPar, fontPar, intPar);

	return task;
}

UkagakaTask::operator UkagakaTaskWait() 
{
	return UkagakaTaskWait(intPar);
}

UkagakaTask::operator UkagakaTaskEnd() 
{
	return UkagakaTaskEnd();
}

UkagakaTask::operator UkagakaTaskNewPhase() {
	return UkagakaTaskNewPhase();
}

HRESULT UkagakaRenderer::HideBalloon()
{
	this->ShowBalloon = false;

	return S_OK;
}

HRESULT UkagakaRenderer::ClearBuffer()
{
	this->taskBuffer = queue<UkagakaTask>();

	return S_OK;
}

HRESULT UkagakaRenderer::MainLogicUpdate()
{

	if (pDirect2DRenderer != NULL)
	{
		LPCSTR uid = this->pDirect2DRenderer->UkagakaID.c_str();

		if (MT_OnGeneralRender != nullptr)
		{
			MT_OnGeneralRender(uid);
		}

		if (bitmapQueue.empty())
		{
			if (MT_OnAnimFinishPlay != nullptr)
			{
				MT_OnAnimFinishPlay(uid);
			}
			if (changing)
			{
				currentAnimation->FetchToQueue(bitmapQueue);
				currentAnimState = nextAnimState;
				changing = false;
			}
			else if (currentAnimState == UWAnimationState::EndWithLastFrame)
			{
				bitmapQueue.push(LastFrame);
			}
			else if (currentAnimState == UWAnimationState::InfinityLoop)
			{
				currentAnimation->FetchToQueue(bitmapQueue);
			}
		}

		if (this->pDirect2DRenderer->IsBufferEmpty())
		{
			array<int, 8> frame = bitmapQueue.front();
			LastFrame = frame;

			RenderTask task = RenderTask();

			//vector<UPRenderElement> animElements(8);

			for (int i = 0; i < sizeof(frame) / sizeof(frame[0]); i++)
			{
				if (frame[i] != 0)
				{
					CPBitmap bm = pDirect2DRenderer->UWBitmapResrouces[frame[i]];

					RenderElement_Bitmap *element = new UWRenderElement_Bitmap({0, 0}, 1.0f, 1.0f, bm);
					task.AddElement(element);
				}
			}
			bitmapQueue.pop();

			if (ShowBalloon)
			{
				RenderElement_Bitmap *balloon = new UWRenderElement_Bitmap({125, 10}, 0.7f, 1.0f,
																		   pDirect2DRenderer->GetBitmapByTag("balloon-default"));
				task.AddElement(balloon);
			}

			if (cacheBalloonText.size() > 0)
			{
				if (CharPerFrame == 1)
				{
					if (TextOutputWaitTick <= 0)
					{
						if (currentBalloonTextIndex < cacheBalloonText.size())
						{
							currentBalloonText += cacheBalloonText[currentBalloonTextIndex];
							currentBalloonTextIndex++;
						}
						else
						{
							if (taskBuffer.size() > 0)
							{
								UkagakaTask task = taskBuffer.front();
								UkagakaTaskType type = task.TaskType;

								if (type == NewPhase)
								{
									this->ClearText();
								}

								switch (type)
								{
								case -1:
								{
									this->currentStatusEnd = true;
									break;
								}
								case 1:
								{
									//this->ClearText();
									UkagakaTaskTextOutput t = (UkagakaTaskTextOutput)task;

									this->textFormats.push_back({ {(UINT32)currentBalloonTextIndex, t.GetString().size()}, t.GetFont() });
									this->textColorBrush = pDirect2DRenderer->UWDefinedBrushResources[t.GetColor()];

									this->ShowBalloon = true;
									this->cacheBalloonText += t.GetString();
									break;
								}
								case 2:
									this->TextOutputWaitTick += static_cast<UkagakaTaskWait>(task).GetTicks();
									break;
								case 3:
								{
									UkagakaTaskAnimation ta = static_cast<UkagakaTaskAnimation>(task);
									if (!ta.IsImmediate()) {
										this->PlayAnimation(ta.GetAnimation(), ta.GetState());
									}
									else {
										this->PlayAnimationImmediately(ta.GetAnimation(), ta.GetState());
									}
									break;
								}
								default:
									Error(L"Undefined task type");
									break;
								}

								taskBuffer.pop();
							}
						}
					}
					else
					{
						TextOutputWaitTick -= 1;
					}

					try
					{
						UWRenderElement_Text *textElement = new UWRenderElement_Text(
							{175, 50}, {125, 250}, 1.0f,
							currentBalloonText, pDirect2DRenderer->TextStyleResources[UWTextStyle::paragraph],
							(CPBrush)this->textColorBrush);

						textElement->formats = textFormats;

						task.AddElement(textElement);
					}
					catch (exception e)
					{
					}
				}
				else
				{
					//TODO: Handle other CharPerFrame situation(different print speed)
				}
			}
			else
			{
				if (taskBuffer.size() > 0)
				{
					UkagakaTask task = taskBuffer.front();
					UkagakaTaskType type = task.TaskType;

					if (type == NewPhase)
					{
						this->ClearText();
					}

					switch (type)
					{
					case -1:
					{
						this->currentStatusEnd = true;
						break;
					}
					case 1:
					{
						//this->ClearText();
						UkagakaTaskTextOutput t = (UkagakaTaskTextOutput)task;

						this->textFormats.push_back({ { (UINT32)currentBalloonTextIndex, t.GetString().size() }, t.GetFont() });
						this->textColorBrush = pDirect2DRenderer->UWDefinedBrushResources[t.GetColor()];

						this->ShowBalloon = true;
						this->cacheBalloonText += t.GetString();
						break;
					}
					case 2:
						this->TextOutputWaitTick += static_cast<UkagakaTaskWait>(task).GetTicks();
						break;
					case 3:
					{
						UkagakaTaskAnimation ta = static_cast<UkagakaTaskAnimation>(task);
						if (!ta.IsImmediate()) {
							this->PlayAnimation(ta.GetAnimation(), ta.GetState());
						}
						else {
							this->PlayAnimationImmediately(ta.GetAnimation(), ta.GetState());
						}
						break;
					}
					default:
						Error(L"Undefined task type");
						break;
					}

					taskBuffer.pop();
				}
			}
			this->pDirect2DRenderer->FetchBuffer(task);
		}

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}