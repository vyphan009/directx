#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Mesh.h"
#include "Camera.h"
#include "Mirror.h"
#include <math.h>
#include <string>
#include "Snow.h"

class GameInit {
protected:
	LPDIRECT3D9 pD3D = 0;//COM object
	LPDIRECT3DDEVICE9 pDevice = 0;//graphics device
	HWND hWndMain;//handle to main window
	LPDIRECT3DSURFACE9 pBackSurf = 0, pSurf = 0, pStretchedImage = 0;//surfaces
	D3DPRESENT_PARAMETERS d3dpp;//rendering info
	D3DDISPLAYMODE d3ddm;//current display mode info
	int fps;
	LPD3DXFONT font;
	RECT rect;
	
	//assignment 2
	Camera camera;
	Mesh mesh[6];
	D3DMATERIAL9 material;
	D3DLIGHT9 light;

	int rotationIndex = -1;
	D3DXVECTOR3 rotationAxis;
	int spotPos;

	//assignment 3
	Mirror mirror0, mirror1, mirror2, mirror3,mirror4, mirror5;
	Snow snow[1000];
	int movIndex = -1;
	D3DXVECTOR3 movPos;

public:
	GameInit();
	~GameInit();
	

	int InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice);
	int init(HWND hwnd);
	int GameShutdown();


	//rendering
	int Render();
	int initBitmap(int width, int height);
	void initFont();
	void setFPS(int fps);
	void FPS();

	static long WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	
	
	//mesh
	void AmbientLight(bool on);
	void PointLight(bool on);
	void SpotLight(bool on);
	void DirectionalLight(bool on);
	void lightInit();
	void projectionInit();
	
	//moving model
	void modelLeftRight(D3DXVECTOR3 pos);
	void modelLeftRight(int index, D3DXVECTOR3 pos);
	void modelUpDown(D3DXVECTOR3 pos);
	void modelUpDown(int index, D3DXVECTOR3 pos);
	void modelForBack(float distance);

	//moving camera around itself
	void cameraRRight(float distance);
	void cameraRUp(float distance);
	void cameraRLook(float distance);
	
	//move camera around model
	void cameraLeftRight(float distance);
	void cameraUpDown(float distance);
	void cameraForBack(float distance);

	//transform mesh
	void startRotate(int index, const D3DXVECTOR3& axis);
	void rotateMesh(const D3DXVECTOR3& axis, float angle);
	void setSpotPos(int pos);

	//assigment 3
	int picking(int x, int y);

	

};
