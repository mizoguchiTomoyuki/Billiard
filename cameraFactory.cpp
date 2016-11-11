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
	newCameraLink->next = nullptr; //�Ƃ肠�����k�������Ă���next��null�Ȃ�Ō�ɒǉ����ꂽ���̂ɂȂ�
	if (LastCameraLink == nullptr){ //�������ŏ��̈�ڂȂ��
		LastCameraLink = newCameraLink;//�ŏI�Ԓn��������
		FirstCameraLink = newCameraLink;
	}
	else{
		LastCameraLink->next = newCameraLink; //�O��̍ŏI�Ԓn��next�����ǉ��������̂�
		newCameraLink->previous = LastCameraLink; //�O��̍ŏI�Ԓn�𒼑O�̃����N��
		LastCameraLink = newCameraLink;
	}
	obj._id = id;
	OBJIDPAIR NewpointPair(id, newCameraLink);
	m_IDMap.insert(NewpointPair);
	return id;
}

void CameraFactory::ReleaseEntity(ELEMID id){

	if (m_IDMap[id]->next == nullptr && m_IDMap[id]->previous == nullptr){//���͂ɉ����Ȃ��ꍇ
		LastCameraLink = nullptr;
		FirstCameraLink = nullptr;
	}
	else if (m_IDMap[id]->next == nullptr || m_IDMap[id]->previous == nullptr){

		//�O��ɎQ�Ƃ��ׂ����̂��Ȃ��ꍇ(Last,First�̂ǂ��炩)
		if (m_IDMap[id]->next == nullptr){ //�ŏI�Ԓn�Ȃ��
			LastCameraLink = m_IDMap[id]->previous;
			m_IDMap[id]->previous->next = nullptr;
		}
		else if (m_IDMap[id]->previous == nullptr){
			FirstCameraLink = m_IDMap[id]->next;
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
void CameraFactory::ClearAll()
{
	m_IDMap.clear();
}
//�S�I�u�W�F�N�g�̃����[�X
void CameraFactory::ReleaseAll(){
	int size = Getsize();
	CameraLink* ent = GetFirstLink();
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
