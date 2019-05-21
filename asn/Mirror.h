#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Mesh.h"

struct CUSTOMVERTEX {
	float x, y, z;
	DWORD color;
};

#define VERTEX_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE) 
class Mirror {
protected:
	D3DXPLANE plane;
	Mesh object ;
	D3DXMATRIX reflect;
	LPDIRECT3DDEVICE9 pDevice;
	IDirect3DVertexBuffer9 *pVertexBuffer;
	D3DXVECTOR3 point;
	
public:
	Mirror();
	~Mirror();

	void  init(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* points, int num, DWORD color);
	bool isInFront(Mesh &obj);
	void draw(Mesh *objs, int num);
	
	void initCube();
};
