#pragma once
//#include <windows.h>
//#include <tchar.h>
//#include <stdio.h>
//#include <d3d9.h>
//#include <d3dx9.h>
#include "GameLoop.h"
#include "GameInit.h"
#include <time.h>

// constructor & destructors
GameInit::GameInit() {};
GameInit::~GameInit() {
	GameShutdown();
}


int GameInit::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
	HRESULT r = 0;

	if (*ppDevice)
		(*ppDevice)->Release();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(r)) {
		return E_FAIL;
	}

	d3dpp.BackBufferWidth = bWindowed ? Width : d3ddm.Width;
	d3dpp.BackBufferHeight = bWindowed ? Height : d3ddm.Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
	if (FAILED(r)) {
		return E_FAIL;
	}

	SetRect(&rect, 0, 0, d3ddm.Width, d3ddm.Height);
	return S_OK;
}

int GameInit::init(HWND hwnd) {
	srand(time(0));
	HRESULT r = 0;

	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D == NULL) {
		return E_FAIL;
	}

	r = InitDirect3DDevice(hwnd, 640, 480, TRUE, D3DFMT_X8R8G8B8, pD3D, &pDevice);
	if (FAILED(r)) {
		return E_FAIL;
	}

	r = initBitmap(d3ddm.Width, d3ddm.Height);
	initFont();
	
	//turn light on
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(0xff, 64, 64, 64));
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//6 objs
	mesh[0].Setup(pDevice, "heli.x");
	mesh[2].Setup(pDevice, "heli.x");
	mesh[4].Setup(pDevice, "heli.x");

	mesh[1].Setup(pDevice, "airplane2.x");
	mesh[3].Setup(pDevice, "airplane2.x");
	mesh[5].Setup(pDevice, "airplane2.x");
		
	//front
	D3DXVECTOR3 pos(-8.f, 0.f, 0.f);
	mesh[0].Translation(pos);
	//top
	pos.x = -8.f;
	pos.y = 20.f;
	pos.z = 30.f;
	mesh[2].Translation(pos);

	//back
	pos.x = -8.f;
	pos.y = 0.f;
	pos.z = 50.f;
	mesh[4].Translation(pos);

	//right
	pos.x = 8.f;
	pos.y = 0.f;
	pos.z = 30.f;
	mesh[1].Translation(pos);
	//left
	pos.x = -30.f;
	pos.y = 0.f;
	pos.z = 30.f;
	mesh[3].Translation(pos);
	
	//bottom
	pos.x = -8.f;
	pos.y = -20.f;
	pos.z = 30.f;
	mesh[5].Translation(pos);



	/*Cube mirror*/
	D3DXMATRIX viewMatrix;
	camera.setPosition(new D3DXVECTOR3(-8.f, 10.f, -60.f));
	camera.getViewMatrix(&viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
	projectionInit();

	//face1 - front
	D3DXVECTOR3 p1(-20.f, 10.f, 20.f);
	D3DXVECTOR3 p2(-20.f, -10.f, 20.f);
	D3DXVECTOR3 p3(0.f, -10.f, 20.f);
	D3DXVECTOR3 p4(0.f, 10.f, 20.f);	
	D3DXVECTOR3 face0[] = { p2, p1, p3, p4};
	mirror0.init(pDevice, face0, 4, D3DCOLOR_XRGB(191, 244, 66));

	//face2 - top - blue
	D3DXVECTOR3 p5(-20.f, 10.f, 40.f);
	D3DXVECTOR3 p6(0.f, 10.f, 40.f);
	D3DXVECTOR3 face1[] = { p1, p5, p4, p6};
	mirror1.init(pDevice, face1, 4, D3DCOLOR_XRGB(0, 0, 255));

	//face3 - back -green
	D3DXVECTOR3 p7(-20.f, -10.f, 40.f);
	D3DXVECTOR3 p8(0.f, -10.f, 40.f);
	D3DXVECTOR3 face2[] = { p5, p7, p6, p8 };
	mirror2.init(pDevice, face2, 4, D3DCOLOR_XRGB(0, 255, 0));

	//face4 - bottom - red
	D3DXVECTOR3 face3[] = { p2, p3, p7, p8 };
	mirror3.init(pDevice, face3, 4, D3DCOLOR_XRGB(255, 0, 0));

	//face5 - right - white
	D3DXVECTOR3 face4[] = { p3, p4, p8, p6 };
	mirror4.init(pDevice, face4, 4, D3DCOLOR_XRGB(255, 255, 255));

	//face6 - left - pink
	D3DXVECTOR3 face5[] = { p2, p7, p1, p5};
	mirror5.init(pDevice, face5, 4, D3DCOLOR_XRGB(198, 12, 235));

	/*Partial system - Snowflakes*/
	for (int i = 0; i < 1000; i++) {
		D3DXVECTOR3 s1(rand() % 100 - 50, 20.f + rand() % 10 + (i / 10) * 10, rand() % 50);
		D3DXVECTOR3 s2(rand() % 100- 50, 20.f + rand() % 10 + (i / 10) * 10, rand() % 50);
		D3DXVECTOR3 s3(rand() % 100 -50, 20.f + rand() % 10 + (i / 10) * 10, rand() % 50);
		D3DXVECTOR3 s4(rand() % 100 - 50, 20.f + rand() % 10 + (i / 10) * 10, rand() % 50);
		D3DXVECTOR3 s5(rand() % 100 - 50, 20.f + rand() % 10 + (i / 10) * 10, rand() % 50);
		D3DXVECTOR3 s6(rand() % 100 -50, 20.f + rand() % 10 + (i / 10) * 10, rand() % 50);
		D3DXVECTOR3 snowflakes[] = { s1, s2, s3, s4, s5, s6 };
		snow[i].initSnow(pDevice, snowflakes, 6);
	}

	




	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	return S_OK;
}

int GameInit::picking(int x, int y) {
	D3DXMATRIX prj;
	pDevice->GetTransform(D3DTS_PROJECTION, &prj);

	int index = 0;
	
	float px = ((2.0f*x/ d3dpp.BackBufferWidth) - 1.f) / prj(0, 0);
	float py = ((-2.0f*y/ d3dpp.BackBufferHeight) + 1.f) / prj(1, 1);

	D3DXVECTOR3 ray_origin = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 ray_direction = D3DXVECTOR3(px, py, 1.f);
	D3DXVECTOR3 wo, wd;
	D3DXMATRIX view;
	camera.getViewMatrix(&view);
	D3DXMatrixInverse(&view, NULL, &view);
	D3DXVec3TransformCoord(&wo, &ray_origin, &view);
	D3DXVec3TransformNormal(&wd, &ray_direction, &view);
	D3DXVec3Normalize(&wd, &wd);

	for (int i = 0; i < 6; i++) {
		float a = wd.x*wd.x + wd.y*wd.y + wd.z*wd.z;
		float b = 2 * (wd.x * (wo.x - mesh[i].getX()) + wd.y * (wo.y - mesh[i].getY()) + wd.z * (wo.z - mesh[i].getZ()));
		float r2 = 3.0f*3.0f;
		float c = pow((wo.x - mesh[i].getX()), 2) + pow((wo.y - mesh[i].getY()), 2) + pow((wo.z - mesh[i].getZ()), 2) - r2;		
		if (((pow(b, 2) - 4*a*c) >= 0)) {
			index = i;
		}
		
	}
	
	return index;
}
int GameInit::GameShutdown() {
	//release resources
	if (pStretchedImage) {
		pStretchedImage->Release();
		pStretchedImage = 0;
	}
	if (font) {
		font->Release();
		font = 0;
	}
	if (pDevice) {
		pDevice->Release();
		pDevice = 0;
	}
	if (pD3D) {
		pD3D->Release();
		pD3D = 0;
	}

	return S_OK;
}


int GameInit::Render() {
	HRESULT r;
	if (!pDevice) {
		return E_FAIL;
	}

	//clear the display arera with colour black
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 25), 1.0f, 0);

	//get pointer to backbuffer
	r = pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurf);
	if (FAILED(r)) {
		
	}
	//update to backbuffer
	r = pDevice->UpdateSurface(pStretchedImage, NULL, pBackSurf, NULL);
	
	//Display FPS
	FPS();

	pBackSurf->Release();
	pBackSurf = 0;

	//draw mesh
	for (int i = 0; i < 6; ++i)
	{
		if (i == rotationIndex)
		{
			D3DXMATRIX rotation;
			D3DXMatrixRotationAxis(&rotation, &rotationAxis, 0.01f);
			mesh[i].Rotate(rotation);
		}
		mesh[i].Draw(pDevice);

	}

	//cube mirror
	mirror0.draw(mesh, 6);
	mirror1.draw(mesh, 6);
	mirror2.draw(mesh, 6);
	mirror3.draw(mesh, 6);
	mirror4.draw(mesh, 6);
	mirror5.draw(mesh, 6);

	//snow
	for (int i = 0; i < 1000; i++) {
		snow[i].update(1.f);
		snow[i].draw();
	}
	
	pDevice->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

//set fps 
void GameInit::setFPS(int fps) {
	this->fps = fps;
}

//create font
void GameInit::initFont() {
	HRESULT r;
	r = D3DXCreateFont(pDevice, 50, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);
}

//draw fps
void GameInit::FPS() {
	pDevice->BeginScene();
	font->DrawText(NULL, (CHAR*) std::to_string(fps).c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);
	pDevice->EndScene();
}



//load bitmap to surface
int LoadBitmapToSurface(const char* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {
	HRESULT r;
	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap == NULL) {
		return E_FAIL;
	}

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);

	//create surface for bitmap
	r = pDevice->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH, ppSurface, NULL);
	if (FAILED(r)) {
		return E_FAIL;
	}

	//load bitmap onto surface
	r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(r)) {
		return E_FAIL;
	}

	return S_OK;
}

//load image to bitmap
int GameInit::initBitmap(int width, int height) {
	HRESULT r;

	r = LoadBitmapToSurface("baboon.bmp", &pSurf, pDevice);

	pDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &pStretchedImage, NULL);

	r = D3DXLoadSurfaceFromSurface(pStretchedImage, NULL, NULL, pSurf, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);

	return S_OK;
}

//----------------------------------------------- asn2 ------------------------------------------------------------------//
long GameInit::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {

	GameLoop &loop = GameLoop::getInstance();
	
	switch (uMessage) {
	case WM_CREATE:
	{
		return 0;
	}
	case WM_PAINT:
	{
		ValidateRect(hWnd, NULL);
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	
	case WM_KEYDOWN:
	{
		loop.keyTracker();
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		loop.mouseTracker(lParam);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	}
}

//set lightings
void GameInit::AmbientLight(bool on) {
	DWORD ambient;
	if (on == true) {
		ambient = D3DCOLOR_XRGB(255, 255, 255);
	}
	else {
		ambient = D3DCOLOR_XRGB(0, 0, 0);
	}
	pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
}

void GameInit::PointLight(bool on) {
	light.Type = D3DLIGHT_POINT;    // make the light type 'point light'
	

	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, on);
}

void GameInit::SpotLight(bool on) {
	light.Type = D3DLIGHT_SPOT;    // make the light type 'spot light'
	if (spotPos == 0) {
		light.Position = D3DXVECTOR3(-8.0f, 10.0f, 0.0f);
	}
	else {
		light.Position = D3DXVECTOR3(8.0f, 10.0f, 0.0f);
	}

		
	pDevice->SetLight(1, &light);
	pDevice->LightEnable(1, on);
}

void GameInit::DirectionalLight(bool on) {
	light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(1.f, 1.0f, 1.0f, 1.0f);    // set the light's color
	light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);

	pDevice->SetLight(2, &light);
	pDevice->LightEnable(2, on);
}

//init light's properties
void GameInit::lightInit() {
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	ZeroMemory(&light, sizeof(&light));
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	light.Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	light.Range = 100.0f;    
	light.Attenuation0 = 0.0f;    
	light.Attenuation1 = 0.125f;    
	light.Attenuation2 = 0.0f;
	light.Phi = D3DXToRadian(90.0f);    // set the outer cone to 30 degrees
	light.Theta = D3DXToRadian(20.0f);    // set the inner cone to 10 degrees
	light.Falloff = 1.0f;    // use the typical falloff


	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pDevice->SetMaterial(&material);
}

//initialize projection view
void GameInit::projectionInit() {
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

//move model
void GameInit::modelUpDown(D3DXVECTOR3 pos) {
	D3DXVECTOR3 translateMatrix;

	for (int i = 0; i < 6; i++) {
		translateMatrix = mesh[i].getTranslation();

		mesh[i].Translation(pos + translateMatrix);
	}
	
}

void GameInit::modelUpDown(int index, D3DXVECTOR3 pos) {
	
	
	D3DXVECTOR3 translateMatrix;

	translateMatrix = mesh[index].getTranslation();
	mesh[index].Translation(pos + translateMatrix);
	mesh[index].Draw(pDevice);
}

void GameInit::modelLeftRight(D3DXVECTOR3 pos) {
	
	D3DXVECTOR3 translateMatrix;

	for (int i = 0; i < 6; i++) {
		translateMatrix = mesh[i].getTranslation();

		mesh[i].Translation(pos + translateMatrix);
	}

}

void GameInit::modelLeftRight(int index, D3DXVECTOR3 pos) 
{
	D3DXVECTOR3 translateMatrix;

	translateMatrix = mesh[index].getTranslation();
	mesh[index].Translation(pos + translateMatrix);
	mesh[index].Draw(pDevice);

}


void GameInit::modelForBack(float distance) {
	camera.walk(distance);
	D3DXMATRIX viewMatrix;
	camera.getViewMatrix(&viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

};

//move camera around model
void GameInit::cameraUpDown(float distance) {
	camera.fly(distance);
	D3DXMATRIX viewMatrix;
	camera.getViewMatrix(&viewMatrix);	
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

}

void GameInit::cameraForBack(float distance) {
	camera.walk(distance);
	D3DXMATRIX viewMatrix;
	camera.getViewMatrix(&viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

};

void GameInit::cameraLeftRight(float distance) {
	camera.strafe(distance);
	D3DXMATRIX viewMatrix;
	camera.getViewMatrix(&viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

}

//moving camera
void GameInit::cameraRRight(float distance) {
	camera.yaw(distance);
	D3DXMATRIX viewMatrix;
	camera.getViewMatrix(&viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

}
void GameInit::cameraRUp(float distance) {
	camera.pitch(distance);
	D3DXMATRIX viewMatrix;
	camera.getViewMatrix(&viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
};

void GameInit::cameraRLook(float distance) {
	camera.roll(distance);
	D3DXMATRIX viewMatrix;
	camera.getViewMatrix(&viewMatrix);
	pDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
}


//transform mesh
void GameInit::startRotate(int index, const D3DXVECTOR3& axis)
{
	rotationIndex = index;
	rotationAxis = axis;
}

void GameInit::rotateMesh(const D3DXVECTOR3& axis, float angle)
{
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &axis, angle);
	mesh[0].Rotation(rotation);
}

void GameInit::setSpotPos(int pos) {
	spotPos = pos;
}