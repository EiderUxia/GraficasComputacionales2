#pragma once
#ifndef _userInputs
#define _userInputs

#include "DXRR.h"
#include <WinUser.h>

#define Key_W 0x57
#define Key_A 0x41
#define Key_S 0x53
#define Key_D 0x44

#define VK_UP 0x26
#define VK_DOWN 0x28
#define P_KEY 0x50

int keypress = 0;


void KeyPressed(DXRR* instance) {
	instance->izqder = 0.0;
	instance->vel = 0.0;

	instance->arriaba = 0.0;

	if (GetAsyncKeyState(Key_A) & 0x8000)
		instance->izqder = -0.01;
	if (GetAsyncKeyState(Key_D) & 0x8000)
		instance->izqder = 0.01;
	if (GetAsyncKeyState(Key_W) & 0x8000)
		instance->vel = 5;
	if (GetAsyncKeyState(Key_S) & 0x8000)
		instance->vel = -5;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		instance->arriaba = 0.01;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		instance->arriaba = -0.01;
	if (GetAsyncKeyState(P_KEY) & 0x8000)
		instance->Posicion();
}




#endif