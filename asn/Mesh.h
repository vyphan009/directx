#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <iostream>
//#include "UIcomponent.h"

#include <Windows.h>
#include <atlbase.h>
#include <iostream>

class Mesh {
protected:

	LPD3DXMESH mesh = nullptr;
	LPD3DXBUFFER adj = nullptr;
	LPD3DXBUFFER mat = nullptr;
	LPD3DXBUFFER effectInstances = nullptr;

	DWORD pNumMaterials;
	D3DMATERIAL9* pMeshMaterials = NULL;
	LPDIRECT3DTEXTURE9* pMeshTextures = NULL;

	//vectors and matrices
	D3DXVECTOR3 positionVector;
	D3DXVECTOR3 scalingVector;
	D3DXMATRIX rotation;
	D3DXMATRIX translation;
	D3DXMATRIX scaling;
	D3DXMATRIX world;
	void Transform();

public:
	Mesh();
	~Mesh();
	//set up mesh properties
	void Setup(LPDIRECT3DDEVICE9 pDevice, const char* filename);
	//draw mesh
	void Draw(LPDIRECT3DDEVICE9 pDevice);
	

	void Rotate(const D3DXMATRIX& r);
	//rotation mesh
	void Rotation(const D3DXMATRIX& r);
	//translate mesh
	void Translation(const D3DXVECTOR3& t);
	//scalling mesh
	void Scaling(const D3DXVECTOR3& s);

	//get translation matrix
	D3DXVECTOR3 getTranslation();
	//get transform matrix
	D3DXMATRIX& getTransform();
	//set transform matrix
	void setTransfrom(D3DXMATRIX &transform);
	
	//get x,y,z
	float getX();
	float getY();
	float getZ();
	
	//inv world matrix
	D3DXMATRIX invWorld();
;};