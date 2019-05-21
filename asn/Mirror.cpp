#include "Mirror.h"
Mirror::Mirror(){}

void Mirror::init(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* points, int num, DWORD color) {
	this->pDevice = pDevice;
	point = points[0];
	pDevice->CreateVertexBuffer(num * sizeof(CUSTOMVERTEX), 0, VERTEX_FVF, D3DPOOL_DEFAULT, &pVertexBuffer, 0);

	CUSTOMVERTEX *vertices;
	pVertexBuffer->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < num; i++) {
		vertices[i].x = points[i].x;
		vertices[i].y = points[i].y;
		vertices[i].z = points[i].z;
		vertices[i].color = color;
	}
	pVertexBuffer->Unlock();
	D3DXPlaneFromPoints(&plane, &points[0], &points[1], &points[2]);
	D3DXMatrixReflect(&reflect, &plane);
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
}
Mirror::~Mirror() {};
bool Mirror::isInFront(Mesh &obj) {
	
	D3DXVECTOR3 objPos(obj.getX(), obj.getY(), obj.getZ());
	objPos -= point;
	return D3DXPlaneDotNormal(&plane, &objPos) > 0 ? true : false;
	
}

void Mirror::draw(Mesh *objs, int num) {
	pDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0F, 0);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//draw mirror
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	pDevice->SetTransform(D3DTS_WORLD, &world);
	pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	pDevice->SetFVF(VERTEX_FVF);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	//draw reflect 
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	for (int i = 0; i < num; i++) {
		bool inFront = isInFront(objs[i]);
		if (inFront == true) {
			D3DXMATRIX oldTransform = objs[i].getTransform();

			D3DXMatrixMultiply(&objs[i].getTransform(), &objs[i].getTransform(), &reflect);
			objs[i].Draw(pDevice);

			objs[i].setTransfrom(oldTransform);

		}
	}
	pDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//void Mirror::initCube() {
//	CUSTOMVERTEX vertices[] =
//};
