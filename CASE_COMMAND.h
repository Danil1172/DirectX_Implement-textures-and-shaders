#pragma once
#include "ALL_LIB.h"
#include  "ThrowField.h"
#include  "FILE_SAVE.h"
#include "RenderObject.h"

extern std::queue<WindowMessage> windowMessages;
//
extern LRESULT LBUTTONDOWN(HWND , UINT , WPARAM , LPARAM );
extern void MOUSEMOVE(HWND , UINT , WPARAM , LPARAM );
extern LRESULT LBUTTONUP(HWND , UINT , WPARAM , LPARAM );
extern void KEYDOWN(HWND , UINT , WPARAM , LPARAM );
