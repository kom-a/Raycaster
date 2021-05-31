#include <stdio.h>
#include <windows.h>
#include <Windowsx.h>

LRESULT CALLBACK WindowProc(_In_ HWND Window, 
							_In_ UINT Message, 
							_In_ WPARAM wParam, 
							_In_ LPARAM lParam)
{
	LRESULT Result = 0;

	switch (Message)
	{
	case WM_MOUSEMOVE:
	{
		
	} break;
	case WM_QUIT:
	{
		// exit(0);
	} break;
	case WM_PAINT:
	{
		PAINTSTRUCT Paint;
		HDC DeviceContext = BeginPaint(Window, &Paint);
		int X = Paint.rcPaint.left;
		int Y = Paint.rcPaint.top;
		int Width = Paint.rcPaint.right - X;
		int Height = Paint.rcPaint.bottom - Y;
		static DWORD Operation = WHITENESS;
		PatBlt(DeviceContext, X, Y, Width, Height, Operation);
		if (Operation == WHITENESS)
			Operation = BLACKNESS;
		else
			Operation = WHITENESS;
		EndPaint(Window, &Paint);
	} break;
	default:
	{
		Result = DefWindowProc(Window, Message, wParam, lParam);	
	}
	}
	return Result;
}

int WINAPI WinMain(	_In_ HINSTANCE Instance, 
					_In_opt_ HINSTANCE PrevInstance,
					_In_ LPSTR CommandLine, 
					_In_ int ShowCode)
{
	WNDCLASS WindowClass = {};

	WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.hInstance = Instance;
	WindowClass.lpszClassName = L"RaycasterWindowClass";

	if (!RegisterClass(&WindowClass))
	{
		// TODO: Handle this
	}

	HWND WindowHandle =  CreateWindowExA(
										0,
										"RaycasterWindowClass",
										"Raycaster",
										WS_OVERLAPPEDWINDOW | WS_VISIBLE,
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										0,
										0,
										Instance,
										0);
	if (!WindowHandle)
	{

	}

	for(;;)
	{
		MSG Message;
		BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
		if (MessageResult > 0)
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			break;
		}
	}


	return 0;
}