#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>

struct SNOW_VERTEX {
	float x, y, z;
	DWORD color;
};

#define SNOW_VERTEX_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE) 
class Snow {
protected:
	LPDIRECT3DDEVICE9 pDevice;
	IDirect3DVertexBuffer9 *pVertexBuffer;
	D3DXPLANE plane;
	int numPlane;

public:
	Snow();
	~Snow();
	void initSnow(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* points,int numPlane);
	void draw();
	void update(float elapsed);
};