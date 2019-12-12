#include "CASE_COMMAND.h"

LRESULT LBUTTONDOWN(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowMessage msg;
	msg.msg = message;
	msg.lparam = lParam;
	msg.wparam = wParam;

	windowMessages.push(msg);
	return 0;
}
void MOUSEMOVE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowMessage msg;
	msg.msg = message;
	msg.lparam = lParam;

	msg.wparam = wParam;

	windowMessages.push(msg);
}
LRESULT LBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	WindowMessage msg;
	msg.msg = message;
	msg.lparam = lParam;

	msg.wparam = wParam;
	windowMessages.push(msg);
	return 0;

}
void KEYDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (wParam & VK_LSHIFT)
	{

		SAVE_FILE();
		MessageBox(hWnd, L"OK", L"DRAW_SAVE", MB_OK);
		WindowMessage msg;
		msg.msg = message;
		msg.lparam = lParam;
		msg.wparam = wParam;

		windowMessages.push(msg);
	}

	if (wParam & 0x44)
	{
		WindowMessage msg;
		msg.msg = message;
		msg.lparam = lParam;
		
		
		msg.wparam = wParam;
		windowMessages.push(msg);
	
	}

}