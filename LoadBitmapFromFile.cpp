#include "LoadBitmapFromFile.h"

HRESULT LoadBitmapFromFile(ID2D1RenderTarget *pRenderTarget, IWICImagingFactory *pIWICFactory, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap **ppBitmap)
{
	ComPtr<IWICBitmapDecoder> decoder;
	ComPtr<IWICBitmapFrameDecode> bitmapSource;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;
	HRESULT hr;

	hr = pIWICFactory->CreateDecoderFromFilename(
		L"C://Users//User/source/repos/tutorial2/tutorial2/Image/BMP_IMAGE/DODGE2.bmp",
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder
	);


	hr = decoder->GetFrame(0, &bitmapSource);
	ThrowField(hr);
	hr = pIWICFactory->CreateFormatConverter(&pConverter);
	ThrowField(hr);
	if (SUCCEEDED(hr))
	{

		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = bitmapSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}
				hr = pIWICFactory->CreateBitmapScaler(&pScaler);

				hr = pScaler->Initialize(
					bitmapSource.Get(),
					destinationWidth,
					destinationHeight,
					WICBitmapInterpolationModeCubic
				);
				ThrowField(hr);

				hr = pConverter->Initialize(
					pScaler,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
				ThrowField(hr);
			}
		}
		else
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.f,
				WICBitmapPaletteTypeMedianCut
			);
		}
	}



	hr = pRenderTarget->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		ppBitmap
	);
	ThrowField(hr);
	if (decoder) decoder->Release();
	if (pSource) pSource->Release();
	if (pStream) pStream->Release();
	if (pScaler) pScaler->Release();

	return hr;


}