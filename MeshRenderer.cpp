#include "MeshRenderer.h"
//コンストラクタ
MeshRenderer::MeshRenderer(){
	enable = true;
	_mesh.isWire = false;
}
//デストラクタ
MeshRenderer::~MeshRenderer(){
	onLostDevice();
}

void MeshRenderer::start(gameObject* t){
	Renderer::start(t);

}

void MeshRenderer::update(){
	_mesh.position = transform->getPosition();
	_mesh.Angle = transform->getRotation();
	_mesh.scale = transform->getScale();


}

void MeshRenderer::initialize(const char *filename, const char *texdir, COLOR_ARGB transcolor, D3DCOLORVALUE MAmb){
	file = filename;
	texdirectory = texdir;
	t_color = transcolor;
	graphics->loadXMesh(filename, transcolor, _mesh, texdir);

	_mesh.MAmbient = MAmb;
	initialized = true;

}

void MeshRenderer::draw(){
	if (!initialized && !enable)
		return;
	graphics->drawMesh(_mesh);

}

void MeshRenderer::onLostDevice(){
	if (!initialized)
		return;
	delete[] _mesh.Texture;
	SAFE_RELEASE(_mesh.Mesh);
	SAFE_RELEASE(_mesh.pMaterials);
	
}

void MeshRenderer::onResetDevice(){
	if (!initialized)
		return;
	graphics->loadXMesh(file, t_color, _mesh, texdirectory);


}