#include "gameObjectFactory.h"
gameObjectFactory *gameObjectFactory::instance = nullptr;
gameObjectFactory::gameObjectLink* gameObjectFactory::LastgameObjectLink = nullptr;
gameObjectFactory::gameObjectLink* gameObjectFactory::FirstgameObjectLink = nullptr;
void gameObjectFactory::Create(){
	if (!instance){
		instance = new gameObjectFactory;
	}
}
bool gameObjectFactory::initialized(){
	bool res = false;
	if (instance){
		res = true;
	}
	return res;
}

void gameObjectFactory::Destroy(){
	delete instance;
	instance = nullptr;
}


ELEMID gameObjectFactory::CreateElem(gameObject &obj){
	ELEMID id = m_IDMap.size();
	for (UINT i = 0; i < m_IDMap.size(); i++){
		auto itr = m_IDMap.find(i);
		if (itr == m_IDMap.end()){
			id = i;
			i = m_IDMap.size() + 1;
		}
		else{

		}
	}
	gameObjectLink* newgameObjectLink = new gameObjectLink;
	newgameObjectLink->id = id;
	newgameObjectLink->pointer = &obj;
	newgameObjectLink->previous = nullptr;
	newgameObjectLink->next = nullptr; //とりあえずヌルを入れておくnextがnullなら最後に追加されたものになる
	if (LastgameObjectLink == nullptr){ //初期化最初の一つ目ならば
		LastgameObjectLink = newgameObjectLink;//最終番地を自分に
		FirstgameObjectLink = newgameObjectLink;
	}
	else{
		LastgameObjectLink->next = newgameObjectLink; //前回の最終番地のnextを今追加したものに
		newgameObjectLink->previous = LastgameObjectLink; //前回の最終番地を直前のリンクに
		LastgameObjectLink = newgameObjectLink;
	}
	obj.setELEMID(id);
	OBJIDPAIR NewpointPair(id, newgameObjectLink);
	m_IDMap.insert(NewpointPair);
	return id;
}

void gameObjectFactory::ReleaseEntity(ELEMID id){

	if (m_IDMap[id]->next == nullptr && m_IDMap[id]->previous == nullptr){//周囲に何もない場合
		LastgameObjectLink = nullptr;
		FirstgameObjectLink = nullptr;
	}
	else if (m_IDMap[id]->next == nullptr || m_IDMap[id]->previous == nullptr){

		//前後に参照すべきものがない場合(Last,Firstのどちらか)
		if (m_IDMap[id]->next == nullptr){ //最終番地ならば
			LastgameObjectLink = m_IDMap[id]->previous;
			m_IDMap[id]->previous->next = nullptr;
		}
		else if (m_IDMap[id]->previous == nullptr){
			FirstgameObjectLink = m_IDMap[id]->next;
			m_IDMap[id]->next->previous = nullptr;
		}
	}
	else{ //両サイドに何かある場合
		m_IDMap[id]->next->previous = m_IDMap[id]->previous;
		m_IDMap[id]->previous->next = m_IDMap[id]->next;
	}


	if (m_IDMap[id]->pointer != nullptr)
		delete (m_IDMap[id]->pointer);

	m_IDMap[id]->pointer = nullptr;

	if (m_IDMap[id] != nullptr)
		delete (m_IDMap[id]);




	m_IDMap[id] = nullptr;
	m_IDMap.erase(id);

}

// 全オブジェクトクリア
void gameObjectFactory::ClearAll()
{
	m_IDMap.clear();
}
//全オブジェクトのリリース
void gameObjectFactory::ReleaseAll(){
	int size = Getsize();
	gameObjectLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){

		int entityid = ent->id; //idを控える
		if (ent->next != nullptr) //次のidを知るためにnextを控える
			ent = ent->next;
		else
			i = size + 1; //ループ処理から抜ける


		ReleaseEntity(entityid);//全部控えた後で削除

	}
}

//死亡オブジェクトの確認->リリース
void gameObjectFactory::Optimize(){
	int size = Getsize();
	gameObjectLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){
		int entityid = ent->id; //idを控える
		if (ent->next != nullptr) //次のidを知るためにnextを控える
			ent = ent->next;
		else
			i = size + 1; //ループ処理から抜ける

		if (GetElem(entityid)->isDestroy()){
			ReleaseEntity(entityid);//DestroyFlagが立っているならば
		}

	}
}