#include "lightFactory.h"
LightFactory *LightFactory::instance = nullptr;
LightFactory::LightLink* LightFactory::LastLightLink = nullptr;
LightFactory::LightLink* LightFactory::FirstLightLink = nullptr;
void LightFactory::Create(){
	if (!instance){
		instance = new LightFactory;
	}
}


void LightFactory::Destroy(){
	delete instance;
	instance = nullptr;
}


ELEMID LightFactory::CreateElem(Light &obj){
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
	LightLink* newLightLink = new LightLink;
	newLightLink->id = id;
	newLightLink->pointer = &obj;
	newLightLink->previous = nullptr;
	newLightLink->next = nullptr; //とりあえずヌルを入れておくnextがnullなら最後に追加されたものになる
	if (LastLightLink == nullptr){ //初期化最初の一つ目ならば
		LastLightLink = newLightLink;//最終番地を自分に
		FirstLightLink = newLightLink;
	}
	else{
		LastLightLink->next = newLightLink; //前回の最終番地のnextを今追加したものに
		newLightLink->previous = LastLightLink; //前回の最終番地を直前のリンクに
		LastLightLink = newLightLink;
	}
	obj.setELEMID(id);
	OBJIDPAIR NewpointPair(id, newLightLink);
	m_IDMap.insert(NewpointPair);
	return id;
}

void LightFactory::ReleaseEntity(ELEMID id){

	if (m_IDMap[id]->next == nullptr && m_IDMap[id]->previous == nullptr){//周囲に何もない場合
		LastLightLink = nullptr;
		FirstLightLink = nullptr;
	}
	else if (m_IDMap[id]->next == nullptr || m_IDMap[id]->previous == nullptr){

		//前後に参照すべきものがない場合(Last,Firstのどちらか)
		if (m_IDMap[id]->next == nullptr){ //最終番地ならば
			LastLightLink = m_IDMap[id]->previous;
			m_IDMap[id]->previous->next = nullptr;
		}
		else if (m_IDMap[id]->previous == nullptr){
			FirstLightLink = m_IDMap[id]->next;
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
void LightFactory::ClearAll()
{
	m_IDMap.clear();
}

//全オブジェクトのリリース
void LightFactory::ReleaseAll(){
	int size = Getsize();
	LightLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){

		int entityid = ent->id; //idを控える
		if (ent->next != nullptr) //次のidを知るためにnextを控える
			ent = ent->next;
		else
			i = size + 1; //ループ処理から抜ける


		ReleaseEntity(entityid);//全部控えた後で削除

	}
}