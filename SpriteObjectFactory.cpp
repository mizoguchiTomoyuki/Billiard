#include "SpriteObjectFactory.h"
SpriteObjectFactory *SpriteObjectFactory::instance = nullptr;
SpriteObjectFactory::SpriteObjectLink* SpriteObjectFactory::LastSpriteObjectLink = nullptr;
SpriteObjectFactory::SpriteObjectLink* SpriteObjectFactory::FirstSpriteObjectLink = nullptr;
void SpriteObjectFactory::Create(){
	if (!instance){
		instance = new SpriteObjectFactory;
	}
}
bool SpriteObjectFactory::initialized(){
	bool res = false;
	if (instance){
		res = true;
	}
	return res;
}

void SpriteObjectFactory::Destroy(){
	delete instance;
	instance = nullptr;
}


ELEMID SpriteObjectFactory::CreateElem(SpriteObject &obj){
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
	SpriteObjectLink* newSpriteObjectLink = new SpriteObjectLink;
	newSpriteObjectLink->id = id;
	newSpriteObjectLink->pointer = &obj;
	newSpriteObjectLink->previous = nullptr;
	newSpriteObjectLink->next = nullptr; //とりあえずヌルを入れておくnextがnullなら最後に追加されたものになる
	if (LastSpriteObjectLink == nullptr){ //初期化最初の一つ目ならば
		LastSpriteObjectLink = newSpriteObjectLink;//最終番地を自分に
		FirstSpriteObjectLink = newSpriteObjectLink;
	}
	else{
		LastSpriteObjectLink->next = newSpriteObjectLink; //前回の最終番地のnextを今追加したものに
		newSpriteObjectLink->previous = LastSpriteObjectLink; //前回の最終番地を直前のリンクに
		LastSpriteObjectLink = newSpriteObjectLink;
	}
	obj.setELEMID(id);
	OBJIDPAIR NewpointPair(id, newSpriteObjectLink);
	m_IDMap.insert(NewpointPair);
	return id;
}

void SpriteObjectFactory::ReleaseEntity(ELEMID id){

	if (m_IDMap[id]->next == nullptr && m_IDMap[id]->previous == nullptr){//周囲に何もない場合
		LastSpriteObjectLink = nullptr;
		FirstSpriteObjectLink = nullptr;
	}
	else if (m_IDMap[id]->next == nullptr || m_IDMap[id]->previous == nullptr){

		//前後に参照すべきものがない場合(Last,Firstのどちらか)
		if (m_IDMap[id]->next == nullptr){ //最終番地ならば
			LastSpriteObjectLink = m_IDMap[id]->previous;
			m_IDMap[id]->previous->next = nullptr;
		}
		else if (m_IDMap[id]->previous == nullptr){
			FirstSpriteObjectLink = m_IDMap[id]->next;
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
void SpriteObjectFactory::ClearAll()
{
	m_IDMap.clear();
}
//全オブジェクトのリリース
void SpriteObjectFactory::ReleaseAll(){
	int size = Getsize();
	SpriteObjectLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){

		int sid = ent->id; //idを控える
		if (ent->next != nullptr) //次のidを知るためにnextを控える
			ent = ent->next;
		else
			i = size + 1; //ループ処理から抜ける


		ReleaseEntity(sid);//全部控えた後で削除

	}
}

//死亡オブジェクトの確認->リリース
void SpriteObjectFactory::Optimize(){
	int size = Getsize();
	SpriteObjectLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){
		int sid = ent->id; //idを控える
		if (ent->next != nullptr) //次のidを知るためにnextを控える
			ent = ent->next;
		else
			i = size + 1; //ループ処理から抜ける

		if (GetElem(sid)->isDestroy()){
			ReleaseEntity(sid);//DestroyFlagが立っているならば
		}

	}
}