#ifndef _MESHREND_H
#define _MESHREND_H
#include "Renderer.h"
class MeshRenderer : public Renderer{
protected:
	MeshData _mesh;
	const char *file;
	const char *texdirectory;
	COLOR_ARGB t_color;
	bool visible;
	D3DXVECTOR3 offset_position; //二個以上のメッシュを持つオブジェクトの内部で違いを出すとき
public:
	MeshRenderer();
	~MeshRenderer();
	virtual void start(gameObject* t);
	virtual void update();
	virtual void draw();
	void initialize(const char *filename, const char *texdir, COLOR_ARGB transcolor, D3DCOLORVALUE MAmb);
	void onLostDevice();
	void onResetDevice();
	void onWireFrame(){ _mesh.isWire = true; }
	void offWireFrame(){ _mesh.isWire = false; }
	void isVisible(bool b){ visible = b; }
	void Setoffset(D3DXVECTOR3 off){ offset_position = off; }

};

#endif