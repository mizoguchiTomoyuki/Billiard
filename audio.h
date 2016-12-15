#ifndef AUDIO_H
#define AUDIO_H
#define WIN32_LEAN_AND_MEAN

#define INTERVAL 0.1f
#include <xact3.h>
#include "constants.h"
class Audio{
protected:
	IXACT3Engine* xactEngine;
	IXACT3WaveBank* waveBank;
	IXACT3SoundBank* soundBank;
	XACTINDEX cueI;
	void* mapWaveBank; //callUnmapViewOfFile() to releaseFile
	void* soundBankData;
	bool coInitialized;
	float intervalCounter;
	bool goplay; //�A�������Đ���H���~�߂�
public:
	Audio();
	virtual ~Audio();
	HRESULT initialize();
	void playCue(const char cue[]);
	void stopCue(const char cue[]);
	void run();

};
#endif