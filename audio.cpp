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
//デストラクタ
//=================================
Audio::~Audio(){
	//XACTをシャットダウン
	if (xactEngine)
	{
		//XACTエンジンをシャットダウンして、リソースを解放
		xactEngine->ShutDown();
		xactEngine->Release();
	}
	if (soundBankData)
		delete[] soundBankData;
	soundBankData = NULL;
	//xactEngine->ShutDown()から戻った後、メモリマップトファイルを解放
	if (mapWaveBank)
		UnmapViewOfFile(mapWaveBank);
	mapWaveBank = NULL;
	if (coInitialized) //CoInitializeExが成功した場合
		CoUninitialize();
}
//=================================
//initialize(初期化)
//=================================
HRESULT Audio::initialize(){
	HRESULT result = E_FAIL;
	HANDLE hFile;
	DWORD fileSize;
	DWORD bytesRead;
	HANDLE hMapFile;
	//XACT3CreateEngine(
	//dwCreationFrags     _ エンジンをどのように作成するか今回は0（通常作成）を指定
	//ppEngine			作成されたエンジンを受け取るアドレス。
	//)
	result = XACT3CreateEngine(0, &xactEngine);
	if (FAILED(result) || xactEngine == NULL)
		return E_FAIL;
	//XACT_RUNTIME_PARAMETERS構造体に値を入れてその値でXACTランタイムの初期化と作成
	XACT_RUNTIME_PARAMETERS xactParams = { 0 }; //初期化に利用する構造体
	xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	result = xactEngine->Initialize(&xactParams);
	if (FAILED(result))
		return result;
	//メモリマップとファイルIOを使用する「インメモリ」のXACTウェイブバンクファイルを作成
	result = E_FAIL; //失敗をデフォルト
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
				//mapWaveBankがファイルへのハンドルを保持
				//不審なハンドルは閉じる
				CloseHandle(hMapFile);

			}
		}
		//mapWaveBankがファイルへのハンドルを保持しているので、
		//不審なハンドルは閉じる
		CloseHandle(hFile);
	}
	if (FAILED(result))
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	//XACTで作成したサウンドバンクファイルを読み取り、登録
	result = E_FAIL;//失敗をデフォルト
	hFile = CreateFile(SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, NULL);
		if (fileSize != -1)
		{
			//サウンドバンク用のメモリを予約
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
//キューで指定されたサウンドをサウンドバンクから再生
//キューが存在しない場合は、サウンドが再生されないだけでエラーは発生しない
//===================================================================================================
void Audio::playCue(const char cue[])
{
	if (!goplay)
		return;
	if (soundBank == NULL)
		return;
	//サウンドバンクからキューインデックスを取得
	cueI = soundBank->GetCueIndex(cue);
	soundBank->Play(cueI, 0, 0, NULL);
	goplay = false;
}
//===================================================================================================
//キューで指定されたサウンドをサウンドバンクで停止
//キューが存在しない場合は、エラーは発生しない
//===================================================================================================
void Audio::stopCue(const char cue[])
{
	if (soundBank == NULL)
		return;
	//サウンドバンクからキューインデックスを取得
	cueI = soundBank->GetCueIndex(cue);
	soundBank->Stop(cueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}
//===================================================================================================
//サウンドエンジンの周期的タスクを実行
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