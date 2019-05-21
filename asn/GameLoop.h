#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameInit.h"
#include "Mirror.h"
#include "windowsx.h"

class GameLoop {
private:
	GameInit init;
	int checkInit;
	__int64 startTime, endTime, diff;
	int counter = 0;
	bool ambient = true, point = true, spot = true, directional = true;
	GameLoop() = default;
	int index;
	bool movAll = true;
	float translationZ = 1.f;
public:
	static GameLoop& getInstance();
	void loopInit(HWND hwnd);
	int loop();
	int getCheckInit();
	void keyTracker();
	void mouseTracker(LPARAM lParam);
};