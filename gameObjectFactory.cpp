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
	newgameObjectLink->next = nullptr; //�Ƃ肠�����k�������Ă���next��null�Ȃ�Ō�ɒǉ����ꂽ���̂ɂȂ�
	if (LastgameObjectLink == nullptr){ //�������ŏ��̈�ڂȂ��
		LastgameObjectLink = newgameObjectLink;//�ŏI�Ԓn��������
		FirstgameObjectLink = newgameObjectLink;
	}
	else{
		LastgameObjectLink->next = newgameObjectLink; //�O��̍ŏI�Ԓn��next�����ǉ��������̂�
		newgameObjectLink->previous = LastgameObjectLink; //�O��̍ŏI�Ԓn�𒼑O�̃����N��
		LastgameObjectLink = newgameObjectLink;
	}
	obj.setELEMID(id);
	OBJIDPAIR NewpointPair(id, newgameObjectLink);
	m_IDMap.insert(NewpointPair);
	return id;
}

void gameObjectFactory::ReleaseEntity(ELEMID id){

	if (m_IDMap[id]->next == nullptr && m_IDMap[id]->previous == nullptr){//���͂ɉ����Ȃ��ꍇ
		LastgameObjectLink = nullptr;
		FirstgameObjectLink = nullptr;
	}
	else if (m_IDMap[id]->next == nullptr || m_IDMap[id]->previous == nullptr){

		//�O��ɎQ�Ƃ��ׂ����̂��Ȃ��ꍇ(Last,First�̂ǂ��炩)
		if (m_IDMap[id]->next == nullptr){ //�ŏI�Ԓn�Ȃ��
			LastgameObjectLink = m_IDMap[id]->previous;
			m_IDMap[id]->previous->next = nullptr;
		}
		else if (m_IDMap[id]->previous == nullptr){
			FirstgameObjectLink = m_IDMap[id]->next;
			m_IDMap[id]->next->previous = nullptr;
		}
	}
	else{ //���T�C�h�ɉ�������ꍇ
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

// �S�I�u�W�F�N�g�N���A
void gameObjectFactory::ClearAll()
{
	m_IDMap.clear();
}
//�S�I�u�W�F�N�g�̃����[�X
void gameObjectFactory::ReleaseAll(){
	int size = Getsize();
	gameObjectLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){

		int entityid = ent->id; //id���T����
		if (ent->next != nullptr) //����id��m�邽�߂�next���T����
			ent = ent->next;
		else
			i = size + 1; //���[�v�������甲����


		ReleaseEntity(entityid);//�S���T������ō폜

	}
}

//���S�I�u�W�F�N�g�̊m�F->�����[�X
void gameObjectFactory::Optimize(){
	int size = Getsize();
	gameObjectLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){
		int entityid = ent->id; //id���T����
		if (ent->next != nullptr) //����id��m�邽�߂�next���T����
			ent = ent->next;
		else
			i = size + 1; //���[�v�������甲����

		if (GetElem(entityid)->isDestroy()){
			ReleaseEntity(entityid);//DestroyFlag�������Ă���Ȃ��
		}

	}
}