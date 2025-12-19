#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc
(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	static char Text[256] = "";
	static int TextLength = 0;

	static int Width = 200;
	static int Height = 200;

	static int Sizes = 20;
	static boolean Command = FALSE;

	switch (msg)
	{
		case WM_SIZE:
		{
			Width  = LOWORD(lParam);
			Height = HIWORD(lParam);

			InvalidateRect(hWnd, NULL, TRUE);
			// store or use width/height
			return 0;
		}

		case WM_CHAR:
		{
			if (Command == TRUE)
			{
				switch ((char)wParam)
				{
					case '+':
						Sizes += 5;
						Command = FALSE;
						break;

					case '-':
						Sizes -= 5;
						Command = FALSE;
						break;
					
					default:
						Command = FALSE;
						break;
				}
			}
			else if (wParam == '\b')
			{
				if (TextLength > 0) Text[--TextLength] = '\0';
			}
			else if (wParam == '`') Command = TRUE;
			else
			{
				Text[TextLength++] = (char)wParam;

				Text[TextLength] = '\0';
			}

			InvalidateRect(hWnd, NULL, TRUE); // Force redraw

			return 0;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			HFONT hFont = CreateFont
			(
				-Sizes, 0, 0, 0,
				FW_NORMAL,
				FALSE, 
				FALSE, 
				FALSE,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				FIXED_PITCH | FF_MODERN,
				"Garamond"
			);

			HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
			HBRUSH hBrush = CreateSolidBrush(RGB(25, 25, 25));

			RECT Rect = {Width / 4, Height / 8, Width - Width / 4, Height - Height / 8}; // Define drawing rectangle

			DrawText(hdc, Text, TextLength, &Rect, DT_LEFT | DT_TOP | DT_WORDBREAK);
			FrameRect(hdc, &Rect, hBrush);

			HFONT GUIFont = CreateFont
			(
				-30, 0, 0, 0,
				FW_NORMAL,
				FALSE, 
				FALSE, 
				FALSE,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				FIXED_PITCH | FF_MODERN,
				"Garamond"
			);

			HFONT oldGUIFont = (HFONT)SelectObject(hdc, GUIFont);

			RECT Recta = {20, 20, 120, 60}; // Define drawing rectangle

			char TextSize[32];
			sprintf(TextSize, "%d", Sizes);

			DrawText(hdc, TextSize, strlen(TextSize), &Recta, DT_LEFT | DT_TOP | DT_WORDBREAK);
			FrameRect(hdc, &Recta, hBrush);

			RECT Rectan = {140, 20, 300, 60}; // Define drawing rectangle
			
			DrawText(hdc, "Garamond", 8, &Rectan, DT_LEFT | DT_TOP | DT_WORDBREAK);
			FrameRect(hdc, &Rectan, hBrush);

			SelectObject(hdc, oldFont);
    		DeleteObject(hFont);

			SelectObject(hdc, oldGUIFont);
    		DeleteObject(GUIFont);

			EndPaint(hWnd, &ps);
			return 0;
		}


		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	char* WindowName = "Window";

	WNDCLASS WindowClass = {0};

	WindowClass.lpfnWndProc = WndProc;
	WindowClass.hInstance = hInstance;
	WindowClass.lpszClassName = WindowName;
	WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //Color, white

	RegisterClass(&WindowClass);

	HWND hWindow = CreateWindow
	(
		WindowName,
		WindowName,
		WS_OVERLAPPEDWINDOW,
		100, 100,
		200, 200,
		NULL,
		NULL,
		hInstance,
		NULL

	);

	ShowWindow(hWindow, nCmdShow);

	MSG Message;
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return 0;
}