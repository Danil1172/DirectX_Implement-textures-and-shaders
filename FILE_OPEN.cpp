#include "FILE_OPEN.h"
extern HWND   hButton;
extern HWND                    g_hWnd ;
extern HWND                    g_hWnd1;
 ComPtr<IWICBitmapDecoder> decoder = NULL;
 extern ComPtr<IWICBitmapFrameDecode> bitmapSource;
extern ComPtr<IWICFormatConverter> converter;
extern ID3D11Device*           g_pd3dDevice ;
extern ID3D11ShaderResourceView*   g_pTextureRV ;
extern ID3D11Resource*             g_pResource ;
extern ID3D11Texture2D*            g_pDepthStencil ;
extern ComPtr<IWICImagingFactory> wicFactory;


void FILE_OPEN()
{
	if (hButton)

	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog *pFileOpen;

			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
			if (SUCCEEDED(hr))
			{
				hr = pFileOpen->Show(g_hWnd);
				if (SUCCEEDED(hr))
				{
					IShellItem *pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEEDITING, &pszFilePath);
						if (SUCCEEDED(hr))
						{

							ComPtr<IWICImagingFactory> wicFactory;
							LPCWSTR path = pszFilePath;
							wstring  file = path;

							CoTaskMemFree(pszFilePath);
							InvalidateRect(g_hWnd1, NULL, NULL);

							CREATE();

							hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);
							hr = wicFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);

							hr = decoder->GetFrame(0, &bitmapSource);
							hr = wicFactory->CreateFormatConverter(&converter);


							WICPixelFormatGUID PIXEL_FORMAT_DESCTINATION = GUID_WICPixelFormat32bppPBGRA;
							hr = converter->Initialize(
								bitmapSource.Get(), PIXEL_FORMAT_DESCTINATION,
								WICBitmapDitherTypeNone, nullptr, 0.f,
								WICBitmapPaletteTypeMedianCut);
							ThrowField(hr);
							UINT uiWidth = 0, uiHeight = 0;
							hr = converter->GetSize(&uiWidth, &uiHeight);
							ThrowField(hr);
							UINT bytesPerPixel = 4;
							UINT cbStride = uiWidth * bytesPerPixel;
							UINT cbBufferSize = cbStride * uiHeight;

							std::vector<BYTE> pbBuffer(cbBufferSize);
							BYTE *obj1 = pbBuffer.data();
							hr = converter->CopyPixels(nullptr, cbStride, cbBufferSize, pbBuffer.data());
							ThrowField(hr);
							auto tex = CreateTexture(uiWidth, uiHeight, pbBuffer.data(), cbStride, DXGI_FORMAT_B8G8R8A8_UNORM);


							D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
							shaderResourceViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
							shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
							shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
							shaderResourceViewDesc.Texture2D.MipLevels = 1;

							hr = g_pd3dDevice->CreateShaderResourceView(tex.Get(), &shaderResourceViewDesc, &g_pTextureRV);
							g_pTextureRV->GetResource(&g_pResource);
							g_pResource->QueryInterface(&g_pDepthStencil);

							InvalidateRect(g_hWnd, NULL, TRUE);


						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}



		auto error = GetLastError();
	}


}

void CREATE()
{
	HRESULT hr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);
	hr = wicFactory->CreateDecoderFromFilename(L"D://Visual/tutorial2/tutorial2/Image/WAY1.png", NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	hr = decoder->GetFrame(0, &bitmapSource);

	hr = wicFactory->CreateFormatConverter(&converter);

	WICPixelFormatGUID PIXEL_FORMAT_DESCTINATION = GUID_WICPixelFormat32bppPBGRA;
	hr = converter->Initialize(
		bitmapSource.Get(), PIXEL_FORMAT_DESCTINATION,
		WICBitmapDitherTypeNone, nullptr, 0.f,
		WICBitmapPaletteTypeMedianCut);
	ThrowField(hr);
	UINT uiWidth = 0, uiHeight = 0;
	hr = converter->GetSize(&uiWidth, &uiHeight);
	ThrowField(hr);
	UINT bytesPerPixel = 4;
	UINT cbStride = uiWidth * bytesPerPixel;
	UINT cbBufferSize = cbStride * uiHeight;

	std::vector<BYTE> pbBuffer(cbBufferSize);
	BYTE *obj1 = pbBuffer.data();
	hr = converter->CopyPixels(nullptr, cbStride, cbBufferSize, pbBuffer.data());
	ThrowField(hr);
	auto tex = CreateTexture(uiWidth, uiHeight, pbBuffer.data(), cbStride, DXGI_FORMAT_B8G8R8A8_UNORM);


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = g_pd3dDevice->CreateShaderResourceView(tex.Get(), &shaderResourceViewDesc, &g_pTextureRV);

}