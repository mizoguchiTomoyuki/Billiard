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
	newLightLink->next = nullptr; //�Ƃ肠�����k�������Ă���next��null�Ȃ�Ō�ɒǉ����ꂽ���̂ɂȂ�
	if (LastLightLink == nullptr){ //�������ŏ��̈�ڂȂ��
		LastLightLink = newLightLink;//�ŏI�Ԓn��������
		FirstLightLink = newLightLink;
	}
	else{
		LastLightLink->next = newLightLink; //�O��̍ŏI�Ԓn��next�����ǉ��������̂�
		newLightLink->previous = LastLightLink; //�O��̍ŏI�Ԓn�𒼑O�̃����N��
		LastLightLink = newLightLink;
	}
	obj.setELEMID(id);
	OBJIDPAIR NewpointPair(id, newLightLink);
	m_IDMap.insert(NewpointPair);
	return id;
}

void LightFactory::ReleaseEntity(ELEMID id){

	if (m_IDMap[id]->next == nullptr && m_IDMap[id]->previous == nullptr){//���͂ɉ����Ȃ��ꍇ
		LastLightLink = nullptr;
		FirstLightLink = nullptr;
	}
	else if (m_IDMap[id]->next == nullptr || m_IDMap[id]->previous == nullptr){

		//�O��ɎQ�Ƃ��ׂ����̂��Ȃ��ꍇ(Last,First�̂ǂ��炩)
		if (m_IDMap[id]->next == nullptr){ //�ŏI�Ԓn�Ȃ��
			LastLightLink = m_IDMap[id]->previous;
			m_IDMap[id]->previous->next = nullptr;
		}
		else if (m_IDMap[id]->previous == nullptr){
			FirstLightLink = m_IDMap[id]->next;
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
void LightFactory::ClearAll()
{
	m_IDMap.clear();
}

//�S�I�u�W�F�N�g�̃����[�X
void LightFactory::ReleaseAll(){
	int size = Getsize();
	LightLink* ent = GetFirstLink();
	for (int i = 0; i < size; i++){

		int entityid = ent->id; //id���T����
		if (ent->next != nullptr) //����id��m�邽�߂�next���T����
			ent = ent->next;
		else
			i = size + 1; //���[�v�������甲����


		ReleaseEntity(entityid);//�S���T������ō폜

	}
}