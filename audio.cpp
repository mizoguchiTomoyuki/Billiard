#include "audio.h"
#include "GameSceneManager.h"
Audio::Audio(){
	xactEngine = NULL;
	waveBank = NULL;
	soundBank = NULL;
	cueI = 0;
	mapWaveBank = NULL;         // Call UnmapViewOfFile() to release file
	soundBankData = NULL;

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
		coInitialized = true;
	else
		coInitialized = false;

	intervalCounter = 0;
	goplay = true;
}

//=================================
//�f�X�g���N�^
//=================================
Audio::~Audio(){
	//XACT���V���b�g�_�E��
	if (xactEngine)
	{
		//XACT�G���W�����V���b�g�_�E�����āA���\�[�X�����
		xactEngine->ShutDown();
		xactEngine->Release();
	}
	if (soundBankData)
		delete[] soundBankData;
	soundBankData = NULL;
	//xactEngine->ShutDown()����߂�����A�������}�b�v�g�t�@�C�������
	if (mapWaveBank)
		UnmapViewOfFile(mapWaveBank);
	mapWaveBank = NULL;
	if (coInitialized) //CoInitializeEx�����������ꍇ
		CoUninitialize();
}
//=================================
//initialize(������)
//=================================
HRESULT Audio::initialize(){
	HRESULT result = E_FAIL;
	HANDLE hFile;
	DWORD fileSize;
	DWORD bytesRead;
	HANDLE hMapFile;
	//XACT3CreateEngine(
	//dwCreationFrags     _ �G���W�����ǂ̂悤�ɍ쐬���邩�����0�i�ʏ�쐬�j���w��
	//ppEngine			�쐬���ꂽ�G���W�����󂯎��A�h���X�B
	//)
	result = XACT3CreateEngine(0, &xactEngine);
	if (FAILED(result) || xactEngine == NULL)
		return E_FAIL;
	//XACT_RUNTIME_PARAMETERS�\���̂ɒl�����Ă��̒l��XACT�����^�C���̏������ƍ쐬
	XACT_RUNTIME_PARAMETERS xactParams = { 0 }; //�������ɗ��p����\����
	xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	result = xactEngine->Initialize(&xactParams);
	if (FAILED(result))
		return result;
	//�������}�b�v�ƃt�@�C��IO���g�p����u�C���������v��XACT�E�F�C�u�o���N�t�@�C�����쐬
	result = E_FAIL; //���s���f�t�H���g
	hFile = CreateFile(WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, NULL);
		if (fileSize != -1)
		{
			hMapFile = CreateFileMapping(
				hFile, NULL, PAGE_READONLY, 0, fileSize, NULL);
			if (hMapFile)
			{
				mapWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
				if (mapWaveBank)
					result = xactEngine->CreateInMemoryWaveBank(
					mapWaveBank, fileSize, 0, 0, &waveBank);
				//mapWaveBank���t�@�C���ւ̃n���h����ێ�
				//�s�R�ȃn���h���͕���
				CloseHandle(hMapFile);

			}
		}
		//mapWaveBank���t�@�C���ւ̃n���h����ێ����Ă���̂ŁA
		//�s�R�ȃn���h���͕���
		CloseHandle(hFile);
	}
	if (FAILED(result))
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	//XACT�ō쐬�����T�E���h�o���N�t�@�C����ǂݎ��A�o�^
	result = E_FAIL;//���s���f�t�H���g
	hFile = CreateFile(SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, NULL);
		if (fileSize != -1)
		{
			//�T�E���h�o���N�p�̃�������\��
			soundBankData = new BYTE[fileSize];
			if (soundBankData)
			{
				if (0 != ReadFile(hFile, soundBankData, fileSize,
					&bytesRead, NULL))
					result = xactEngine->CreateSoundBank(
					soundBankData, fileSize, 0, 0, &soundBank);
			}
		}
		CloseHandle(hFile);
	}
	if (FAILED(result))
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	return S_OK;
}

//===================================================================================================
//�L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N����Đ�
//�L���[�����݂��Ȃ��ꍇ�́A�T�E���h���Đ�����Ȃ������ŃG���[�͔������Ȃ�
//===================================================================================================
void Audio::playCue(const char cue[])
{
	if (!goplay)
		return;
	if (soundBank == NULL)
		return;
	//�T�E���h�o���N����L���[�C���f�b�N�X���擾
	cueI = soundBank->GetCueIndex(cue);
	soundBank->Play(cueI, 0, 0, NULL);
	goplay = false;
}
//===================================================================================================
//�L���[�Ŏw�肳�ꂽ�T�E���h���T�E���h�o���N�Œ�~
//�L���[�����݂��Ȃ��ꍇ�́A�G���[�͔������Ȃ�
//===================================================================================================
void Audio::stopCue(const char cue[])
{
	if (soundBank == NULL)
		return;
	//�T�E���h�o���N����L���[�C���f�b�N�X���擾
	cueI = soundBank->GetCueIndex(cue);
	soundBank->Stop(cueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
//===================================================================================================
//�T�E���h�G���W���̎����I�^�X�N�����s
//===================================================================================================
void Audio::run()
{
	float frameTime =  GameSceneManager::Instance().GetGameptr()->getframeTime();
	if (!goplay){
		intervalCounter += frameTime;
		if (intervalCounter > INTERVAL){
			goplay = true;
			intervalCounter = 0;
		}
	}
	if (xactEngine == NULL)
		return;
	xactEngine->DoWork();
}