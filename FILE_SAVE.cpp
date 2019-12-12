#include "FILE_SAVE.h"

extern HWND                    g_hWnd;
extern ComPtr<IWICImagingFactory> wicFactory;
extern ID3D11DeviceContext*    g_pImmediateContext ;
extern Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> pFrameEncode;
extern Microsoft::WRL::ComPtr<IPropertyBag2> pPropertyBag;
extern IDXGISwapChain*         g_pSwapChain ;
extern ID3D11Device*           g_pd3dDevice ;


void FILE_SAVE()
{
	CComPtr<IFileSaveDialog> pFileOpen;
	COMDLG_FILTERSPEC FILE_TYPE[] =
	{
		{L"PNG"  , L"*.png"} ,
		{L"JPG"   , L"*.jpg"} ,
		{L"BMP" , L"*.bmp" } ,
	};

	HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));
	if (SUCCEEDED(hr))
	{
		pFileOpen->SetFileTypes(3, FILE_TYPE);
		pFileOpen->SetTitle(L"A File_SAVE");
		pFileOpen->SetOkButtonLabel(L"OK");
		pFileOpen->SetFileName(L"NAME_IMAGE");
		pFileOpen->SetDefaultExtension(FILE_TYPE->pszName);
		hr = pFileOpen->Show(g_hWnd);
		if (SUCCEEDED(hr))
		{
			IShellItem *pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{

				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				if (SUCCEEDED(hr))
				{
					LPWSTR path = pszFilePath;
					wstring file = path;
					pFileOpen->SetDefaultExtension(path);
					CoTaskMemFree(pszFilePath);
					//InvalidateRect(hWnd, NULL, NULL);
					pFileOpen->SetSaveAsItem(pItem);

					SAVE();
					/////////////////////////////////////////////////////////////////////
					CComPtr<IWICBitmapEncoder> pEncoder;

					HRESULT hr = S_OK;
					hr = wicFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);

					{
						CComPtr<IWICStream> pFileStream;
						hr = wicFactory->CreateStream(&pFileStream);



						std::wstring output = L"C://Users//User/source/repos/tutorial2/tutorial2/chek19.png";
						pFileStream->InitializeFromFilename(output.c_str(), GENERIC_WRITE); //DWORD
						hr = pEncoder->Initialize(pFileStream, WICBitmapEncoderNoCache);// no keshurovaniz
						ThrowField(hr);

						Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;

						g_pImmediateContext->OMGetRenderTargets(1, rtv.GetAddressOf(), nullptr);

						Microsoft::WRL::ComPtr<ID3D11Resource> res;

						rtv->GetResource(res.GetAddressOf());

						Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;

						hr = res.As(&tex);

						D3D11_TEXTURE2D_DESC texDesc;

						tex->GetDesc(&texDesc);


						auto outFmt = GUID_WICPixelFormat32bppPBGRA;
						/// 
						UINT uiWidth = texDesc.Width;
						UINT uiHeight = texDesc.Height;


						UINT bytesPerPixel = 4;
						UINT cbStride = uiWidth * bytesPerPixel;
						UINT cbBufferSize = cbStride * uiHeight;

						std::vector<BYTE> pbBuffer(cbBufferSize);
						BYTE *obj1 = pbBuffer.data();


						pFrameEncode = nullptr;
						pPropertyBag = nullptr;
						hr = pEncoder->CreateNewFrame(&pFrameEncode, &pPropertyBag);

						hr = pFrameEncode->Initialize(nullptr);
						hr = pFrameEncode->SetPixelFormat(&outFmt);
						uiWidth = texDesc.Width;
						uiHeight = texDesc.Height;

						hr = pFrameEncode->SetSize(uiWidth, uiHeight);


						hr = pFrameEncode->WritePixels(uiHeight, cbStride, cbBufferSize, pbBuffer.data());


						hr = pFrameEncode->Commit();// KADR V IMAGE
						hr = pEncoder->Commit();

						auto hResult = SaveWICTextureToFile(g_pImmediateContext, tex.Get(), GUID_ContainerFormatPng, path);


					}


					MessageBox(g_hWnd, path, L"WINDOW", MB_OK);


				};
				pItem->Release();

			}

		}
	}

}

void SAVE()
{

	CComPtr<IWICBitmapEncoder> pEncoder;
	auto tmpFn = [&] {


		HRESULT hr = S_OK;
		hr = wicFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);

		{
			CComPtr<IWICStream> pFileStream;
			hr = wicFactory->CreateStream(&pFileStream);
			if (FAILED(hr))
			{
				return hr;
			}

			std::wstring output = L"C://Users//User/source/repos/tutorial2/tutorial2/chek19.png";
			pFileStream->InitializeFromFilename(output.c_str(), GENERIC_WRITE); //DWORD
			hr = pEncoder->Initialize(pFileStream, WICBitmapEncoderNoCache);// no keshurovaniz
			ThrowField(hr);

			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;

			g_pImmediateContext->OMGetRenderTargets(1, rtv.GetAddressOf(), nullptr);

			Microsoft::WRL::ComPtr<ID3D11Resource> res;

			rtv->GetResource(res.GetAddressOf());

			Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;

			hr = res.As(&tex);

			D3D11_TEXTURE2D_DESC texDesc;

			tex->GetDesc(&texDesc);



			auto outFmt = GUID_WICPixelFormat32bppPBGRA;
			/// 
			UINT uiWidth = texDesc.Width;
			UINT uiHeight = texDesc.Height;

			UINT bytesPerPixel = 4;
			UINT cbStride = uiWidth * bytesPerPixel;
			UINT cbBufferSize = cbStride * uiHeight;

			std::vector<BYTE> pbBuffer(cbBufferSize);
			BYTE *obj1 = pbBuffer.data();


			pFrameEncode = nullptr;
			pPropertyBag = nullptr;
			hr = pEncoder->CreateNewFrame(&pFrameEncode, &pPropertyBag);

			hr = pFrameEncode->Initialize(nullptr);
			hr = pFrameEncode->SetPixelFormat(&outFmt);
			uiWidth = texDesc.Width;
			uiHeight = texDesc.Height;
			hr = pFrameEncode->SetSize(uiWidth, uiHeight);

			hr = pFrameEncode->WritePixels(uiHeight, cbStride, cbBufferSize, pbBuffer.data());
			hr = pFrameEncode->Commit();// KADR V IMAGE
			hr = pEncoder->Commit();

		}

	};
	tmpFn();

}
void SAVE_FILE()
{
	CComPtr<IFileSaveDialog> pFileOpen;
	COMDLG_FILTERSPEC FILE_TYPE[] =
	{
		{L"PNG"  , L"*.png"} ,
		{L"JPG"   , L"*.jpg"} ,
		{L"BMP" , L"*.bmp" } ,
	};

	HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));
	if (SUCCEEDED(hr))
	{
		pFileOpen->SetFileTypes(3, FILE_TYPE);
		pFileOpen->SetTitle(L"A File_SAVE");
		pFileOpen->SetOkButtonLabel(L"OK");
		pFileOpen->SetFileName(L"NAME_IMAGE");
		pFileOpen->SetDefaultExtension(FILE_TYPE->pszName);
		hr = pFileOpen->Show(g_hWnd);
		if (SUCCEEDED(hr))
		{
			IShellItem *pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{

				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				if (SUCCEEDED(hr))
				{
					LPWSTR path = pszFilePath;
					wstring file = path;
					pFileOpen->SetDefaultExtension(path);
					CoTaskMemFree(pszFilePath);
					pFileOpen->SetSaveAsItem(pItem);


					/////////////////////////////////////////////////////////////////////

					ID3D11Texture2D* pBuffer;
					ID3D11Texture2D* pBackBufferStaging = NULL;
					g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBuffer);
					D3D11_TEXTURE2D_DESC td;
					pBuffer->GetDesc(&td);
					td.BindFlags = 0;
					td.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
					td.Usage = D3D11_USAGE_STAGING;
					//td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					//td.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;


					g_pd3dDevice->CreateTexture2D(&td, NULL, &pBackBufferStaging);
					g_pImmediateContext->CopyResource(pBackBufferStaging, pBuffer);

					auto hResult = SaveWICTextureToFile(g_pImmediateContext, pBackBufferStaging, GUID_ContainerFormatBmp, path);

					pBackBufferStaging->Release();
					pBuffer->Release();





					MessageBox(g_hWnd, path, L"WINDOW", MB_OK);


				};
				pItem->Release();

			}

		}
	}



}