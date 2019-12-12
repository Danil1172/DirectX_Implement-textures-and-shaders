#include "CreateD2D.h"

D2D CreateD2D(ID3D11DeviceContext *d3dCtx) {

	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;

	d3dCtx->OMGetRenderTargets(1, rtv.GetAddressOf(), nullptr);

	Microsoft::WRL::ComPtr<ID3D11Resource> res;

	rtv->GetResource(res.GetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;

	hr = res.As(&tex);
	assert(SUCCEEDED(hr));

	Microsoft::WRL::ComPtr<IDXGISurface> surf;

	hr = tex.As(&surf);
	assert(SUCCEEDED(hr));

	
	D2D d2d;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, d2d.factory.GetAddressOf());
	assert(SUCCEEDED(hr));
	D3D11_TEXTURE2D_DESC texDesc;

	tex->GetDesc(&texDesc);

	auto rtProps = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(texDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED));

	hr = d2d.factory->CreateDxgiSurfaceRenderTarget(surf.Get(), rtProps, d2d.ctx.GetAddressOf());

	assert(SUCCEEDED(hr));
	return d2d;


}