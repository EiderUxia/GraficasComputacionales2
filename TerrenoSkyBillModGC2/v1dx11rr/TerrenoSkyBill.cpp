#include <windows.h>
#include <windowsx.h>

#include "DXRR.h"
#include "GamePadRR.h"

#include "Movimiento.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

DXRR *dxrr;
GamePadRR *gamePad;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"DXRR_E1";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, 1280, 800};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
                          L"DXRR_E1",
                          L"Ejemplo 1 de DX11 RR",
                          WS_OVERLAPPEDWINDOW,
                          100,
                          100,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);
	dxrr = new DXRR(hWnd, 1200, 800);
	dxrr->vel=0;
    gamePad = new GamePadRR(1);

	SetTimer(hWnd, 100, 33, NULL);
    MSG msg;

    while(TRUE)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if(msg.message == WM_QUIT)
                break;
        }

        KeyPressed(dxrr);

        dxrr->Render();
    }

    return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_KEYDOWN: {
            if (wParam == VK_ESCAPE){
                KillTimer(hWnd, 100);
                PostQuitMessage(0);
                return 0;
            }
            break;
        }
        case WM_DESTROY:
            {
				KillTimer(hWnd, 100);
                PostQuitMessage(0);
                return 0;
            } break;

		case WM_TIMER:
			{
				if (gamePad->IsConnected())
				{
					dxrr->izqder = 0;
					dxrr->arriaba = 0;

					if (gamePad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
					{
						dxrr->izqder = 0.01;
					}
					

					if (gamePad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
					{
						dxrr->izqder = -0.01;
					}

					if (gamePad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
					{
						dxrr->arriaba = 0.01;
					}
					

					if (gamePad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
					{
						dxrr->arriaba = -0.01;
					}

					dxrr->vel = gamePad->GetState().Gamepad.bLeftTrigger/255.0;
					
				}
			} break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


