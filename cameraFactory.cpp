#include "cameraFactory.h"
CameraFactory *CameraFactory::instance = nullptr;
CameraFactory::CameraLink* CameraFactory::LastCameraLink = nullptr;
CameraFactory::CameraLink* CameraFactory::FirstCameraLink = nullptr;
void CameraFactory::Create(){
	if (!instance){
		instance = new CameraFactory;
	}
}
bool CameraFactory::initialized(){
	bool res = false;
	if (instance){
		res = true;
	}
	return res;
}

void CameraFactory::Destroy(){
	delete instance;
	instance = nullptr;
}


ELEMID CameraFactory::CreateElem(CameraData &obj){
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
	CameraLink* newCameraLink = new CameraLink;
	newCameraLink->id = id;
	newCameraLink->pointer = &obj;
	newCameraLink->previous = nullptr;
	newCameraLink->next = nullptr; //とりあえずヌルを入れておくnextがnullなら最後に追加されたものになる
	if (LastCameraLink == nullptr){ //初期化最初の一つ目ならば
		LastCameraLink = newCameraLink;//最終番地を自分に
		FirstCameraLink = newCameraLink;
	}
	else{
		LastCameraLink->next = newCameraLink; //前回の最終番地のnextを今追加したものに
		newCameraLink->previous = LastCameraLink; //前回の最終番地を直前のリンクに
		LastCameraLink = newCameraLink;
	}
	obj._id = id;
	OBJIDPAIR NewpointPair(id, newCameraLink);
	m_IDMap.insert(NewpointPair);
	return id;
}

void CameraFactory::ReleaseEntity(ELEMID id){

	if (m_IDMap[id]->next == nullptr && m_IDMap[id]->previous == nullptr){//周囲に何もない場合
		LastCameraLink = nullptr;
		FirstCameraLink = nullptr;
	}
	else if (m_IDMap[id]->next == nullptr || m_IDMap[id]->previous == nullptr){

		//前後に参照すべきものがない場合(Last,Firstのどちらか)
		if (m_IDMap[id]->next == nullptr){ //最終番地ならば
			LastCameraLink = m_IDMap[id]->previous;
			m_IDMap[id]->previous->next = nullptr;
		}
		else if (m_IDMap[id]->previous == nullptr){
			FirstCameraLink = m_IDMap[id]->next;
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
void CameraFactory::ClearAll()
{
	m_IDMap.clear();
}
//全オブジェクトのリリース
void CameraFactory::ReleaseAll(){
	int size = Getsize();
	CameraLink* ent = GetFirstLink();
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
