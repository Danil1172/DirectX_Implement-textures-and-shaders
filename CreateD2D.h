#pragma once
#include "resource.h"
#include "ALL_LIB.h"
struct D2D {
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> ctx;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>  ctx1;
	Microsoft::WRL::ComPtr<ID2D1Factory> factory;
};

D2D CreateD2D(ID3D11DeviceContext *sd3dCtx);
