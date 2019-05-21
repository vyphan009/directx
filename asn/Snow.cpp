#include "Snow.h"
Snow::Snow() {};
Snow::~Snow() {};
void Snow::initSnow(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* points, int numPlane) {
	this->pDevice = pDevice;
	this->numPlane = numPlane;
	pDevice->CreateVertexBuffer(numPlane * sizeof(SNOW_VERTEX), 0, SNOW_VERTEX_FVF, D3DPOOL_DEFAULT, &pVertexBuffer, 0);

	SNOW_VERTEX *vertices;
	pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < numPlane; i++) {
		vertices[i].x = points[i].x;
		vertices[i].y = points[i].y;
		vertices[i].z = points[i].z;
		vertices[i].color = D3DCOLOR_XRGB(255, 255, 235);
	}
	pVertexBuffer->Unlock();
	D3DXPlaneFromPoints(&plane, &points[0], &points[1], &points[2]);
}
void Snow::draw() {
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	pDevice->SetTransform(D3DTS_WORLD, &world);
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(SNOW_VERTEX));
	pDevice->SetFVF(SNOW_VERTEX_FVF);
	pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, 2);
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void Snow::update(float elapsed)
{
	SNOW_VERTEX *vertices;
	pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < numPlane; i++) {
		vertices[i].y -= elapsed * 1.f;
	}
	pVertexBuffer->Unlock();
}
