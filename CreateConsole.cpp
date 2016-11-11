#include "CreateConsole.h"
#include "Ball.h"
#include "Box.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "MouseExtend.h"
//======================================================================================
//�R���X�g���N�^
//======================================================================================
CreateConsole::CreateConsole() : DebugWindow()
{
	fontColor = CConsoleNS::FONT_COLOR;
	backColor = CConsoleNS::BACK_COLOR;
	x = CConsoleNS::X;
	y = CConsoleNS::Y;
	selectRowY = 0;
	textRect.bottom = CConsoleNS::Y + CConsoleNS::HEIGHT - CConsoleNS::MARGIN;
	textRect.left = CConsoleNS::X + CConsoleNS::MARGIN;
	textRect.right = CConsoleNS::X + CConsoleNS::WIDTH - CConsoleNS::MARGIN;
	textRect.top = CConsoleNS::Y + CConsoleNS::MARGIN;
	cmode = CConsoleNS::CREATEMODE::SELECTMENU;
	exitRow = 0;
	selectRow = 0;
	MinSelectRow = 0;
	MaxSelectRow = 10; //�������l�͓K��
	Arrow_Key = 0;
	StartValue = 0.0f;
}

//=============================================================================
// Destructor
//=============================================================================
CreateConsole::~CreateConsole()
{
	onLostDevice();            // call onLostDevice() for every graphics item
}

//======================================================================================
//�R���\�[���̏�����
//======================================================================================
bool CreateConsole::initialize(Graphics *g, Input *in)
{
	try{
		graphics = g;  //�O���t�B�b�N�f�o�C�X
		input = in;
		//����
		vtx[0].x = x;
		vtx[0].y = y;
		vtx[0].z = 0.0f;
		vtx[0].rhw = 1.0f;
		vtx[0].dwColor = backColor;
		//�E��
		vtx[1].x = x + CConsoleNS::WIDTH;
		vtx[1].y = y;
		vtx[1].z = 0.0f;
		vtx[1].rhw = 1.0f;
		vtx[1].dwColor = backColor;
		//�E��
		vtx[2].x = x + CConsoleNS::WIDTH;
		vtx[2].y = y + CConsoleNS::HEIGHT;
		vtx[2].z = 0.0f;
		vtx[2].rhw = 1.0f;
		vtx[2].dwColor = backColor;
		//����
		vtx[3].x = x;
		vtx[3].y = y + CConsoleNS::HEIGHT;
		vtx[3].z = 0.0f;
		vtx[3].rhw = 1.0f;
		vtx[3].dwColor = backColor;
		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
		//DirectX�t�H���g��������
		if (dxFont.initialize(graphics, CConsoleNS::FONT_HEIGHT, false, false, CConsoleNS::FONT) == false)
			return false;
		dxFont.setFontColor(fontColor);

	}
	catch (...){
		return false;
	}
	initialized = true;
	return true;
}



//======================================================================================
//�R���\�[����`��
//���s�O�FBeginScene��EndScene�̓����ɂ���
//======================================================================================
const void CreateConsole::draw()
{
	if (!visible || graphics == NULL || !initialized)
		return;
	if (Activate){ //ActivateOnOff����Console����̈ڍs����{�Ȃ̂ł����ŏ��������Ă���.
		selectRowY = 0;
		MinSelectRow = 0;
		MaxSelectRow = 10; //�������l�͓K��
		cmode = CConsoleNS::SELECTMENU;
	}
	DebugWindow::draw();
}
//AppendDraw��DebugWindow����draw�ŌĂяo����Ă���(���Ȃ�spriteBegin-End���ɓ���Ă�����������)
//�Ȃ̂Œǉ��������@�\�����Ă���
const void CreateConsole::Appenddraw(){
	if (rowHeight <= 0)	//�G���[�悯
		rowHeight = 20;
	//�w��̗��">"��\������
	textRect.bottom = (long)(selectRowY + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	std::string prompt = ">";		//�v�����v�g���������쐬
	if (Active ){
	//	prompt = prompt + input->getTextIn();
	}
	dxFont.print(prompt, textRect, DT_LEFT); //�v�����v�g�ƃR�}���h��\��
	switch (cmode){
	case CConsoleNS::SELECTMENU:
	{
		SelectMenu(); 
	}
	break;
	case CConsoleNS::CREATEMENU:
	{
		CreateMenu();
	}
	break;
	case CConsoleNS::EDIT:
	{
		EditMenu();
	}
	break;
	}
}
//SelectMenu�̕`��p
void CreateConsole::SelectMenu(){

	if (rowHeight <= 0)	//�G���[�悯
		rowHeight = 20;

	int MenuRow = 0;

	textRect.bottom = (long)(MenuRow + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	std::string menu = "    Create"; //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_LEFT);

	gameObjectFactory::gameObjectLink* obj = gameObjectFactory::Instance().GetFirstLink();
	int objSize = gameObjectFactory::Instance().Getsize();
	for (int i = 0; i < objSize; i++){
		MenuRow = (i+1)*rowHeight;
		menu = "    " + obj->pointer->GetObjClassName()+ "   " + obj->pointer->GetName();
		Objmap[i + 1] = obj->pointer->getELEMID();
		textRect.bottom = (long)(MenuRow + CConsoleNS::Y + rowHeight);
		textRect.top = textRect.bottom - rowHeight;

		dxFont.print(menu, textRect, DT_LEFT);

		if (obj->next != nullptr)
			obj = obj->next;
		else
			i = objSize + 1;

	}

	exitRow = objSize+1;
	textRect.bottom = (long)(exitRow*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "    exit"; //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);

	MaxSelectRow = exitRow;
	
	textRect.bottom = (long)((exitRow+1)*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "S:" + std::to_string(selectRow) + " E:" + std::to_string(exitRow); //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);


}



//CreateMenu�̕`��p
//Create�ł���prefab��\������
void CreateConsole::CreateMenu(){

	if (rowHeight <= 0)	//�G���[�悯
		rowHeight = 20;

	int MenuRow = 0;

	textRect.bottom = (long)(MenuRow + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	std::string menu = "    Create MENU"; //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_LEFT);

	for (int i = 0; i < PREFAB_MENU::EXIT; i++){
		MenuRow = (i + 1)*rowHeight;
		menu = "    " + PrefabName((PREFAB_MENU)i);

		textRect.bottom = (long)(MenuRow + CConsoleNS::Y + rowHeight);
		textRect.top = textRect.bottom - rowHeight;

		dxFont.print(menu, textRect, DT_LEFT);
	}
	exitRow = PREFAB_MENU::EXIT+1;
	textRect.bottom = (long)(exitRow*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "    exit"; //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);

	MaxSelectRow = exitRow;


	textRect.bottom = (long)((exitRow + 1)*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "name:" + input->getTextIn(); //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);

}

//CreateMenu�̕`��p
//Create�ł���prefab��\������
void CreateConsole::EditMenu(){

	if (rowHeight <= 0)	//�G���[�悯
		rowHeight = 20;

	int MenuRow = 0;
	std::string menu = ""; //CreateMenu�Ɍ�����Menu
	st =  gameObjectFactory::Instance().GetElem(selectObj)->ConsoleInformation();

	
	for (unsigned int i = 0; i < st.size(); i++){
		MenuRow = (i + 1)*rowHeight;
		menu = "  " + st[i].s;

		textRect.bottom = (long)(MenuRow + CConsoleNS::Y + rowHeight);
		textRect.top = textRect.bottom - rowHeight;

		dxFont.print(menu, textRect, DT_LEFT);
	}
	exitRow = st.size() + 1;
	textRect.bottom = (long)(exitRow*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "    exit"; //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);


	textRect.bottom = (long)((exitRow + 1)*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "value:" + input->getTextIn(); //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);


	float Mouse_LDrag = (float)input->getDragStart_X(inputNS::M_LEFT);
	textRect.bottom = (long)((exitRow + 2)*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "valueDS:" + std::to_string(Mouse_LDrag); //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);


	Mouse_LDrag = (float)input->getDragDistance_X(inputNS::M_LEFT);
	textRect.bottom = (long)((exitRow + 3)*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "valueDD:" + std::to_string(Mouse_LDrag); //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);

	Mouse_LDrag = (float)input->getMousePosX();
	textRect.bottom = (long)((exitRow + 4)*rowHeight + CConsoleNS::Y + rowHeight);
	textRect.top = textRect.bottom - rowHeight;
	menu = "Svalue:" + std::to_string(Mouse_LDrag); //CreateMenu�Ɍ�����Menu
	dxFont.print(menu, textRect, DT_RIGHT);

	MaxSelectRow = exitRow;
}
//======================================================================================
//�R���\�[���R�}���h��߂�
//�V���O���L�[�̃R���\�[���R�}���h����������
//���̑��̃R�}���h�͂��ׂăQ�[���ɖ߂�
//======================================================================================
std::string CreateConsole::getCommand()
{

	bool Drag = input->isDraging(inputNS::M_LEFT);
	if (!Active)
		return "";
	//�R���\�[��������������Ă��Ȃ��ꍇ�A�܂��͕\������Ă��Ȃ��ꍇ
	if (!initialized || !visible )
		return "";

	//�R���\�[���L�[���`�F�b�N
	if (input->wasKeyPressed(CONSOLE_KEY))
		hide();
	//Exc�L�[���`�F�b�N
	if (input->wasKeyPressed(ESC_KEY))
		return "exit";
	//�X�N���[�����`�F�b�N
	if (input->wasKeyPressed(VK_UP)){
		scrollAmount++;
		if (selectRow > MinSelectRow){
			selectRowY -= rowHeight;
			selectRow--;
		}
		else{
			selectRow = MaxSelectRow;
			selectRowY = selectRow*rowHeight;
		}
	}
	else if (input->wasKeyPressed(VK_DOWN)){
		scrollAmount--;
		if (selectRow < MaxSelectRow){
			selectRowY += rowHeight;
			selectRow++;
		}
		else{
			selectRow = MinSelectRow;
			selectRowY = selectRow*rowHeight;

		}
	}
	else if (input->wasKeyPressed(VK_PRIOR))
		scrollAmount += rows;
	else if (input->wasKeyPressed(VK_NEXT))
		scrollAmount -= rows;
	if (scrollAmount < 0)
		scrollAmount = 0;
	if (scrollAmount > CConsoleNS::MAX_LINE - 1)
		scrollAmount = CConsoleNS::MAX_LINE - 1;
	if (scrollAmount > (int)(text.size()) - 1)
		scrollAmount = (int)(text.size()) - 1;
	
	//���E�L�[�̓��͒l�A���l�̕ύX�Ȃǂŗ��p����

	if (!Drag){
		if (input->wasKeyPressed(VK_RIGHT)){
			Arrow_Key += 0.01f;
		}
		else if (input->wasKeyPressed(VK_LEFT)){
			Arrow_Key -= 0.01f;
		}
		else{
			Arrow_Key = 0.0f;
		}
		//ArrowKey�̏���l
		if (abs(Arrow_Key) > 1.0f){
			Arrow_Key = SIGN(Arrow_Key)*1.0f;
		}
	}
	EditNumber_fromMouse();
	EditNumber_fromKey();
	commandStr = input->getTextIn();	//���[�U�[�����͂����e�L�X�g���擾
	//�L�[���Q�[���ɓn���Ȃ�
	input->clear(inputNS::KEYS_DOWN | inputNS::KEYS_PRESSED | inputNS::MOUSE);
	if (commandStr.length() == 0)	//�R�}���h�����͂���Ă��Ȃ��ꍇ
		return "";
	//Enter�L�[��������Ă��Ȃ��ꍇ
	if (commandStr.at(commandStr.length() - 1) != '\r')
		return "";					//�߂�(�R�}���h�łȂ�)
	//�R�}���h�̖�������u\r�v������
	commandStr.erase(commandStr.length() - 1);
	input->clearTextIn();	//���͍s���N���A
	inputStr = commandStr;                      // save input text
	commandStr = DesideMenu(inputStr);
	

	return commandStr;
}
//=============================================================================
// called when graphics device is lost
//=============================================================================
void CreateConsole::onLostDevice()
{
	if (!initialized)
		return;
	dxFont.onLostDevice();
	SAFE_RELEASE(vertexBuffer);
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void CreateConsole::onResetDevice()
{
	if (!initialized)
		return;
	graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
	dxFont.onResetDevice();
}


std::string CreateConsole::DesideMenu(std::string s){

	std::string returnValue = s;
	bool digitflag = false;
	bool minusflag = false;
	if (s != ""){
		if (s.at(0) == '-'){ //�����΂�߂�-�Ȃ炻���������MinusFlag���I��
			s.replace(s.cbegin(), s.cbegin()+1, "");
			minusflag = true;
		}
		int index_dot = 0;
		int dotnum = 0;
		for (int i = s.size()-1; i >= 0; i--){ //.�̐��𐔂��Ă����ʓ|�Ȃ̂ł��̂܂�replace������
			index_dot = s.rfind(".", i);
			if (index_dot == std::string::npos){
				i = 0; //���[�v�E�o
			}
			else{
				dotnum++;
				if (dotnum > 1){
					s.replace(i + 1, 1, "");
				}
				i = index_dot;//����.��T��
			}
		}
		if (std::all_of(s.cbegin(), s.cend(), [](char c){return isdigit(c) || (c == '.'); })){ //������.�����Ȃ��digitflag�͂���
			digitflag = true;
		}
	}
	switch (cmode){
	case CConsoleNS::CREATEMODE::SELECTMENU:
	{
		if (selectRow == 0){
			//Create���j���[
			returnValue = ""; //�K�v�O�̃R�}���h�̏d����h��
			cmode = CConsoleNS::CREATEMENU;
			MinSelectRow = 2;
			selectRow = 2;
			selectRowY = selectRow*rowHeight;
		}
		else if (selectRow == exitRow){
			returnValue = "exit"; //��������exit
		}
		else{
			//Edit���j���[��
			returnValue = ""; //�K�v�O�̃R�}���h�̏d����h��
			selectObj = Objmap[selectRow];
			cmode = CConsoleNS::EDIT;
			MinSelectRow = 1;
			selectRow = 2;
			selectRowY = selectRow*rowHeight;
		}
	}
	break;
	case CConsoleNS::CREATEMODE::CREATEMENU:
	{

		if (selectRow == 0){
			returnValue = ""; //�K�v�O�̃R�}���h�̏d����h�� 
		}
		else if (selectRow == exitRow){
			//SelectMenu�ɖ߂� 
			cmode = CConsoleNS::SELECTMENU;
			MinSelectRow = 0;
			selectRow = MinSelectRow;
			selectRowY = MinSelectRow*rowHeight;
		}
		else{
			//���� 
			if (selectRow < exitRow){
				CreatePrefab((PREFAB_MENU)(selectRow - 1), s);
				cmode = CConsoleNS::SELECTMENU;
				returnValue = "";
				MinSelectRow = 0;
				selectRow = MinSelectRow;
				selectRowY = MinSelectRow*rowHeight;
			}
		}
	}
	break;
	case CConsoleNS::CREATEMODE::EDIT:
	{
		if (selectRow-1 >= 0 && selectRow-1 < st.size()){
			if (st[selectRow-1].i == gameObjectNS::INDEX){
				//Create���j���[
				returnValue = "";
			}else{
				//Edit���j���[��
				returnValue = ""; //�K�v�O�̃R�}���h�̏d����h��
				if (st[selectRow - 1].i == gameObjectNS::FLOAT){
					if (digitflag && s != ""){
						gameObject* p = gameObjectFactory::Instance().GetElem(selectObj);
						float num = 0;
						num = std::stof(s);
						if (minusflag)
							num = -num;
						p->setfloat_IndexString(st[selectRow - 1].indexNumber, num);
					}
					
				}
			}
		}else if (selectRow == exitRow){
			cmode = CConsoleNS::SELECTMENU; //��������exit
			returnValue = "";
			selectObj = 0;
			selectRowY = 0;
			MinSelectRow = 0;
			st.clear();
			Objmap.clear();
			selectRow = MinSelectRow;
			selectRowY = MinSelectRow*rowHeight;
		}
	}
	break;
	}	
	return returnValue;

}
//Prefab�̖��O��Ԃ�
std::string CreateConsole::PrefabName(PREFAB_MENU p){
	std::string returnValue = "";
	switch (p){
	case PREFAB_MENU::BALL:
	{
		returnValue = "Ball";
	}
	break;
	case PREFAB_MENU::CAMERA:
	{
		returnValue = "Camera";
	}
	break;
	case PREFAB_MENU::DIRECTIONALLIGHT:
	{
		returnValue = "DirectionalLight";
	}
	break;
	case PREFAB_MENU::POINTLIGHT:
	{
		returnValue = "PointLight";
	}
	break;
	case PREFAB_MENU::BOX:
	{
		returnValue = "Box";
	}
	break;
	case PREFAB_MENU::EXIT:
	{
		returnValue = "exit";
	}
	break;
	default:
		break;

	}
	return returnValue;

}
//Prefab�������s���B
void CreateConsole::CreatePrefab(PREFAB_MENU p,std::string n){


	switch (p){
	case PREFAB_MENU::BALL:
	{
		Ball* b = new Ball();
		b->SetName(n);
	}
	break;
	case PREFAB_MENU::CAMERA:
	{
		Camera* c = new Camera();
		c->SetName(n);
	}
	break;
	case PREFAB_MENU::DIRECTIONALLIGHT:
	{
		DirectionalLight* _dl = new DirectionalLight();
		_dl->SetName(n);
	}
	break;
	case PREFAB_MENU::POINTLIGHT:
	{
		PointLight* _pl = new PointLight();
		_pl->SetName(n);
	}
	break;
	case PREFAB_MENU::BOX:
	{
		Box* _box = new Box();
		_box->SetName(n);
	}
	break;
	case PREFAB_MENU::EXIT:
	{

	}
	break;
	default:
		break;

	}
}

void CreateConsole::EditNumber_fromKey(){
	if (Arrow_Key == 0.0f)
		return;
	switch (cmode){
	case CConsoleNS::CREATEMODE::SELECTMENU:
	{
	}
	break;
	case CConsoleNS::CREATEMODE::CREATEMENU:
	{}
	break;
	case CConsoleNS::CREATEMODE::EDIT:
	{
		if (selectRow - 1 >= 0 && selectRow - 1 < st.size()){
			if (st[selectRow - 1].i == gameObjectNS::FLOAT){//����ȊO��������

				gameObject* p = gameObjectFactory::Instance().GetElem(selectObj);
				float num = p->getfloat_IndexString(st[selectRow - 1].indexNumber)+Arrow_Key;
				p->setfloat_IndexString(st[selectRow - 1].indexNumber, num);
			}else{
				//Edit���j���[��

				
			}
		}
		else if (selectRow == exitRow){
		}
	}
	break;
	default:
		break;
	}

}

void CreateConsole::EditNumber_fromMouse(){
	bool Drag = input->isDraging(inputNS::M_LEFT);
	
	float Mouse_LDrag = input->getDragDistance_X(inputNS::M_LEFT)/10.0f;
	switch (cmode){
	case CConsoleNS::CREATEMODE::SELECTMENU:
	{
	}
	break;
	case CConsoleNS::CREATEMODE::CREATEMENU:
	{}
	break;
	case CConsoleNS::CREATEMODE::EDIT:
	{
		if (selectRow - 1 >= 0 && selectRow - 1 < st.size()){

			if (st[selectRow - 1].i == gameObjectNS::FLOAT){//����ȊO��������

				gameObject* p = gameObjectFactory::Instance().GetElem(selectObj);
				if (!Drag){
					StartValue = p->getfloat_IndexString(st[selectRow - 1].indexNumber);
				}
				else{
					float num = StartValue + Mouse_LDrag;
					p->setfloat_IndexString(st[selectRow - 1].indexNumber, num);
				}
			}
			else{
				//Edit���j���[��


			}
		}
		else if (selectRow == exitRow){
		}
	}
	break;
	default:
		break;
	}

}

