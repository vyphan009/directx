#include "Mesh.h"
//construct and desctruct
Mesh::Mesh()
	: positionVector(0.f, 0.f, 0.f)
	, scalingVector(1.f, 1.f, 1.f)
{
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixScaling(&scaling, scalingVector.x, scalingVector.y, scalingVector.z);
	D3DXMatrixTranslation(&translation, positionVector.x, positionVector.y, positionVector.z);
	Transform();
}

Mesh::~Mesh()
{
	if (pMeshMaterials != NULL) {
		delete[] pMeshMaterials;
		pMeshMaterials = NULL;
	}

	if (pMeshTextures)
	{
		for (DWORD i = 0; i < pNumMaterials; i++)
		{
			if (pMeshTextures[i])
				pMeshTextures[i]->Release();
		}
		delete[] pMeshTextures;
	}
	if (mesh != NULL)
		mesh->Release();
}



void Mesh::Setup(LPDIRECT3DDEVICE9 pDevice, const char* filename) {
	HRESULT hr = D3DXLoadMeshFromX(
		filename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		&adj,
		&mat,
		NULL, 
		&pNumMaterials,
		&mesh);

	//Get the location of the materials
	D3DXMATERIAL* materials = (D3DXMATERIAL*)mat->GetBufferPointer();
	//
	pMeshMaterials = new D3DMATERIAL9[pNumMaterials];
	pMeshTextures = new LPDIRECT3DTEXTURE9[pNumMaterials];

	for (DWORD i = 0; i < pNumMaterials; i++)
	{
		pMeshMaterials[i] = materials[i].MatD3D; //copy them

		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse; //D3DX does not guarantee ambient is to be set
		pMeshTextures[i] = nullptr;

		if (materials[i].pTextureFilename)
		{
			hr = D3DXCreateTextureFromFile(
				pDevice,
				(LPCSTR)materials[i].pTextureFilename,
				&pMeshTextures[i]);
		}
	}
	D3DXMatrixIdentity(&world);
	
}

void Mesh::Rotate(const D3DXMATRIX& r)
{
	D3DXMatrixMultiply(&rotation, &rotation, &r);
	Transform();
}

void Mesh::Rotation(const D3DXMATRIX& r) {
	rotation = r;
	Transform();
};

void Mesh::Scaling(const D3DXVECTOR3& s) {
	scalingVector = s;
	D3DXMatrixScaling(&scaling, scalingVector.x, scalingVector.y, scalingVector.z);
	Transform();
}

void Mesh::Translation(const D3DXVECTOR3& t) {
	positionVector = t;
	D3DXMatrixTranslation(&translation, positionVector.x, positionVector.y, positionVector.z);
	Transform();
}

D3DXVECTOR3 Mesh::getTranslation() {
	return positionVector;
}

void Mesh::Transform() {
	D3DXMatrixMultiply(&world, &scaling, &rotation);
	D3DXMatrixMultiply(&world, &world, &translation);
;}

void Mesh::Draw(LPDIRECT3DDEVICE9 pDevice) {
	pDevice->SetTransform(D3DTS_WORLD, &world);
	
	for (DWORD i = 0; i < pNumMaterials; i++) {
		pDevice->SetMaterial(&pMeshMaterials[i]);
		pDevice->SetTexture(0, pMeshTextures[i]);
		mesh->DrawSubset(i);
	}
}

//assigment 3
D3DXMATRIX& Mesh::getTransform() {
	return world;
}

void Mesh::setTransfrom(D3DXMATRIX &transform) {
	world = transform;
}

float Mesh::getX() {
	return positionVector.x;
}

float Mesh::getY() {
	return positionVector.y;
}

float Mesh::getZ() {
	return positionVector.z;
}


D3DXMATRIX Mesh::invWorld() {
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, NULL, &world);
	return invWorld;
}