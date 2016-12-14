#include "Billiards.h"
#include "SpriteObjectFactory.h"
#include "BilliardTable.h"
Billiard::Billiard(){
	GameSceneManager::Create();
	LightFactory::Create();
	gameObjectFactory::Create();
	SpriteObjectFactory::Create();
	TextureInit = new TextureInitializer();
}

Billiard::~Billiard(){
	delete ODM;
	delete TextureInit;
	deleteAll();
}

void Billiard::initialize(HWND hwnd){
	Game::initialize(hwnd);
	GameSceneManager::Instance().SetGamePtr(this);
	TextureInit->initializeTexture(graphics);
	_bmanager.initialize();
	ODM = new ObjectDataManager();
	ODM->DataLoad();
	_bmanager.GameStart();
	graphics->setBackColor(graphicsNS::BLACK);
	Ang = 0;
	//graphics->SetMainCamera(_cam);
	return;

}

void Billiard::update(){
	_bmanager.update();
	Lightupdate();
	gameObjectupdate();
	SpriteObjectupdate();
	gameObjectFactory::Instance().Optimize();
	SpriteObjectFactory::Instance().Optimize();
}
void Billiard::ai(){


}
void Billiard::collisions(){
	gameObjectFactory::gameObjectLink* obj = gameObjectFactory::Instance().GetFirstLink();
	int mSize = gameObjectFactory::Instance().Getsize();
	for (int i = 0; i < mSize; i++){
			obj->pointer->collision();

		if (obj->next != nullptr)
			obj = obj->next;
		else
			i = mSize + 1;

	}
}
void Billiard::render(){
	graphics->spriteBegin();
	SpriteObjectdraw();
	graphics->spriteEnd();
	graphics->MeshBegin();
	gameObjectdraw();
	graphics->MeshEnd();
}
void Billiard::releaseAll(){
	Game::releaseAll();
	return;

}
void Billiard::resetAll(){
	Game::resetAll();
	return;
}

void Billiard::Lightupdate(){
	if (&LightFactory::Instance() == nullptr)
		return;
	LightFactory::LightLink* light = LightFactory::Instance().GetFirstLink();
	int mSize = LightFactory::Instance().Getsize();
	for (int i = 0; i < mSize; i++){
		light->pointer->update();
		if (light->next != nullptr)
			light = light->next;
		else
			i = mSize + 1;

	}
}

void Billiard::gameObjectupdate(){
	gameObjectFactory::gameObjectLink* obj = gameObjectFactory::Instance().GetFirstLink();
	int mSize = gameObjectFactory::Instance().Getsize();
	for (int i = 0; i < mSize; i++){
		obj->pointer->update();
		if (obj->next != nullptr)
			obj = obj->next;
		else
			i = mSize + 1;

	}
}


void Billiard::gameObjectdraw(){
	gameObjectFactory::gameObjectLink* obj = gameObjectFactory::Instance().GetFirstLink();
	int mSize = gameObjectFactory::Instance().Getsize();
	for (int i = 0; i < mSize; i++){
		obj->pointer->draw();
		if (obj->next != nullptr)
			obj = obj->next;
		else
			i = mSize + 1;

	}
}

void Billiard::SpriteObjectupdate(){
	SpriteObjectFactory::SpriteObjectLink* obj = SpriteObjectFactory::Instance().GetFirstLink();
	int mSize = SpriteObjectFactory::Instance().Getsize();
	for (int i = 0; i < mSize; i++){
		obj->pointer->update();
		if (obj->next != nullptr)
			obj = obj->next;
		else
			i = mSize + 1;

	}
}
void Billiard::SpriteObjectdraw(){
	SpriteObjectFactory::SpriteObjectLink* obj = SpriteObjectFactory::Instance().GetFirstLink();
	int mSize = SpriteObjectFactory::Instance().Getsize();
	for (int i = 0; i < mSize; i++){
		obj->pointer->draw();
		if (obj->next != nullptr)
			obj = obj->next;
		else
			i = mSize + 1;

	}

}

void Billiard::deleteAll(){
	Game::deleteAll();
	GameSceneManager::Destroy();
	LightFactory::Instance().ReleaseAll();
	LightFactory::Instance().Destroy();
	gameObjectFactory::Instance().ReleaseAll();
	gameObjectFactory::Instance().Destroy();
	SpriteObjectFactory::Instance().ReleaseAll();
	SpriteObjectFactory::Instance().Destroy();
	releaseAll(); //すべてのグラフィックスアイテムについてonLostDevice()を呼び出す

}
//全部消して作り直す
void Billiard::ResetGame(){
	LightFactory::Instance().ReleaseAll();
	LightFactory::Instance().Destroy();
	gameObjectFactory::Instance().ReleaseAll();
	gameObjectFactory::Instance().Destroy();
	SpriteObjectFactory::Instance().ReleaseAll();
	SpriteObjectFactory::Instance().Destroy();
	SpriteObjectFactory::Create();
	LightFactory::Create();
	gameObjectFactory::Create();
	ODM = new ObjectDataManager();
	ODM->DataLoad();
	_bmanager.GameStart();

}