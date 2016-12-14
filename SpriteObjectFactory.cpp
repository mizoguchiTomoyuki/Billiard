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
	newSpriteObjectLink->next = nullptr; //�Ƃ肠�����k�������Ă���next��null�Ȃ�Ō�ɒǉ����ꂽ���̂ɂȂ�
	if (LastSpriteObjectLink == nullptr){ //�������ŏ��̈�ڂȂ��
		LastSpriteObjectLink = newSpriteObjectLink;//�ŏI�Ԓn��������
		FirstSpriteObjectLink = newSpriteObjectLink;
	}
	else{
		LastSpriteObjectLink->next = newSpriteObjectLink; //�O��̍ŏI�Ԓn��next�����ǉ��������̂�
		newSpriteObjectLink->previous = LastSpriteObjectLink; //�O��̍ŏI�Ԓn�𒼑O�̃����N��
		LastSpriteObjectLink = newSpriteObjectLink;
	}
	obj.setELEMID(id);
	OBJIDPAIR NewpointPair(id, newSpriteObjectLink);
	m_IDMap.insert(NewpointPair);
	return id;
}

void SpriteObjectFactory::ReleaseEntity(ELEMID id){

	if (m_IDMap[id]->next == nullptr && m_IDMap[id]->previous == nullptr){//���͂ɉ����Ȃ��ꍇ
		LastSpriteObjectLink = nullptr;
		FirstSpriteObjectLink = nullptr;
	}
	else if (m_IDMap[id]->next == nullptr || m_IDMap[id]->previous == nullptr){

		//�O��ɎQ�Ƃ��ׂ����̂��Ȃ��ꍇ(Last,First�̂ǂ��炩)
		if (m_IDMap[id]->next == nullptr){ //�ŏI�Ԓn�Ȃ��
			LastSpriteObjectLink = m_IDMap[id]->previous;
			m_IDMap[id]->previous->next = nullptr;
		}
		else if (m_IDMap[id]->previous == nullptr){
			FirstSpriteObjectLink = m_IDMap[id]->next;
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
void SpriteObjectFactory::ClearAll()
{
	m_IDMap.clear();
}
//�S�I�u�W�F�N�g�̃����[�X
void SpriteObjectFactory::ReleaseAll(){
	int size = Getsize();
	SpriteObjectLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){

		int sid = ent->id; //id���T����
		if (ent->next != nullptr) //����id��m�邽�߂�next���T����
			ent = ent->next;
		else
			i = size + 1; //���[�v�������甲����


		ReleaseEntity(sid);//�S���T������ō폜

	}
}

//���S�I�u�W�F�N�g�̊m�F->�����[�X
void SpriteObjectFactory::Optimize(){
	int size = Getsize();
	SpriteObjectLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){
		int sid = ent->id; //id���T����
		if (ent->next != nullptr) //����id��m�邽�߂�next���T����
			ent = ent->next;
		else
			i = size + 1; //���[�v�������甲����

		if (GetElem(sid)->isDestroy()){
			ReleaseEntity(sid);//DestroyFlag�������Ă���Ȃ��
		}

	}
}