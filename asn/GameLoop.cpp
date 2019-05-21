#include "GameLoop.h"
#include "GameInit.h"

GameLoop& GameLoop::getInstance() {
	static GameLoop gameLoop;
	return gameLoop;
}

void GameLoop::loopInit(HWND hwnd) {
	checkInit = init.init(hwnd);
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

}


int GameLoop::loop() {
	init.Render();
	QueryPerformanceCounter((LARGE_INTEGER*)&endTime);
	counter++;
	diff = endTime - startTime;
	if (diff > 1000000) {
		init.setFPS(counter);
		counter = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	}
	init.lightInit();
	//keyTracker();
	return S_OK;
}

int GameLoop::getCheckInit() {
	return checkInit;
}


void GameLoop::keyTracker() {
	
	if (GetAsyncKeyState(VK_ESCAPE)) {
		PostQuitMessage(0);
	}
	//mesh1 - '1'
	else if (GetKeyState(0x31) & 0x8000) {
		index = 0;
		init.setSpotPos(0);
	}
	//mesh2 - '2'
	else if (GetKeyState(0x32) & 0x8000) {
		index = 1;
		init.setSpotPos(1);
	}
	else if (GetKeyState(0x33) & 0x8000) {
		index = 2;
		init.setSpotPos(2);
	} 
	else if (GetKeyState(0x34) & 0x8000) {
		index = 3;
		init.setSpotPos(3);
	}
	else if (GetKeyState(0x35) & 0x8000) {
		index = 4;
		init.setSpotPos(4);
	}
	else if (GetKeyState(0x36) & 0x8000) {
		index = 5;
		init.setSpotPos(5);
	}

	/*turn light on and off*/
	//ambient - 'A'
	else if (GetKeyState(0x41) & 0x8000) {
		init.AmbientLight(ambient);
		ambient = !ambient;
	}
	//spot - 'S'
	else if (GetKeyState(0x53) & 0x8000) {
		init.SpotLight(spot);
		spot = !spot;
	}
	//directional - 'D'
	else if (GetKeyState(0x44) & 0x8000) {
		init.DirectionalLight(directional);
		directional = !directional;
	}
	//point - 'W'
	else if (GetKeyState(0x57) & 0x8000) {
		init.PointLight(point);
		point = !point;
	}
	
	/*tranform mesh*/
	//up
	else if (GetKeyState(VK_UP) & 0x8000) {
		if (movAll == true) {
			init.modelUpDown(D3DXVECTOR3(0.f, 0.f, 1.f));
		}
		else if (movAll == false){
			init.modelUpDown(index, D3DXVECTOR3(0.f, 0.f, 1.f));
		}
	}
	//down
	else if (GetKeyState(VK_DOWN) & 0x8000) {
		translationZ--;
		if (movAll == true) {

			init.modelUpDown(D3DXVECTOR3(0.f, 0.f, -1.f));
		}
		else {
			init.modelUpDown(index, D3DXVECTOR3(0.f, 0.f, -1.f));

		}
	}
	//right
	else if (GetKeyState(VK_RIGHT) & 0x8000) {
		if (movAll == true) {
			init.modelLeftRight(D3DXVECTOR3(1.f, 0.f, 0.f));
		}
		else {
			init.modelLeftRight(index, D3DXVECTOR3(1.f, 0.f, 0.f));
		}
	}
	//left
	else if (GetKeyState(VK_LEFT) & 0x8000) {
		if (movAll == true) {
			init.modelLeftRight(D3DXVECTOR3(-1.f, 0.f, 0.f));
		}
		else {
			init.modelLeftRight(index, D3DXVECTOR3(-1.f, 0.f, 0.f));

		}
	}
	//move backward - 'H'
	else if (GetKeyState(0X48) & 0x8000) {
		init.modelForBack(-D3DX_PI / 4);
	}
	//move forward - 'U'
	else if (GetKeyState(0X55) & 0x8000) {
		init.modelForBack(D3DX_PI / 4);
	}


	/*move camera around itself*/
	//rotate camera up
	else if (GetKeyState(0x49) & 0x8000) {
		init.cameraRUp(-D3DX_PI / 20);

	}
	//rotate camera down
	else if (GetKeyState(0x4B) & 0x8000) {
		init.cameraRUp(D3DX_PI / 20);

	}
	//rotate camera right J
	else if (GetKeyState(0x4A)& 0x8000) {
		init.cameraRRight(-D3DX_PI / 20);

	}
	//rotate camera left L
	else if (GetKeyState(0x4C) & 0x8000) {
		init.cameraRRight(D3DX_PI / 20);

	}

	/*move camera aound model*/
	//camera move up B
	else if (GetKeyState(0x42) & 0x8000) {
		init.cameraUpDown(-D3DX_PI / 2);
	}
	//camera move down G
	else if (GetKeyState(0x47) & 0x8000) {
		init.cameraUpDown(D3DX_PI / 2);

	}
	//camera move right V
	else if (GetKeyState(0x56) & 0x8000) {
		init.cameraLeftRight(-D3DX_PI / 4);

	}
	// N
	else if (GetKeyState(0x4E) & 0x8000) {
		init.cameraLeftRight(D3DX_PI / 4);

	}

	//rotate model
	else if (GetKeyState(0x4F) & 0x8000) {
		init.cameraRLook(-D3DX_PI / 20);
	}
	//rotate model around x-axis - 'X'
	else if (GetKeyState(0x58) & 0x8000) {
		init.startRotate(index, D3DXVECTOR3(1.f, 0.f, 0.f));
	}
	//rotate model y-axis  - 'Y'
	else if (GetKeyState(0x59) & 0x8000) {
		init.startRotate(index, D3DXVECTOR3(0.f, 1.f, 0.f));
	}
	//rotate model z-axis  - 'Z'
	else if (GetKeyState(0x5A) & 0x8000) {
		init.startRotate(index, D3DXVECTOR3(0.f, 0.f, 1.f));
	}
	//stop rotate
	else if (GetKeyState(VK_SPACE) & 0x8000) {
		init.startRotate(-1, D3DXVECTOR3(1.f, 0.f, 0.f));
	}
	else if (GetKeyState(VK_RETURN) & 0x8000) {
		movAll = !movAll;
	}
}

void GameLoop::mouseTracker(LPARAM lParam) {
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	index = init.picking(xPos, yPos);
	
}