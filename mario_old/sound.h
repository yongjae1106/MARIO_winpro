#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <dsound.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


struct Sound 
{
    LPDIRECTSOUNDBUFFER pBuffer = nullptr;
};

extern Sound jump_small_Sound;
extern Sound jump_big_Sound;
extern Sound fireball_Sound;
extern Sound bowserfire_Sound;
extern Sound kick_Sound;
extern Sound stomp_Sound;

extern Sound bump_Sound;

extern Sound bowserfalls_Sound;
extern Sound bowserdead_Sound;

extern Sound coin_Sound;
extern Sound up_Sound;
extern Sound powerup_Sound;
extern Sound powerup_appears_Sound;
extern Sound powerdown_Sound;

extern Sound die_Sound;
extern Sound gameover_Sound;
extern Sound stage_clear_Sound;
extern Sound world_clear_Sound;

extern bool InitDirectSound(HWND hwnd);
extern LPDIRECTSOUND8 g_pDS;
extern HWND g_hWnd; // 메인 윈도우 핸들 (WinMain에서 넘겨줘야 함)
extern IDirectSoundBuffer* g_pBGMBuffer;

void PlaySoundBuffer(Sound& sound);
void PlayBGM(const char* filename);
Sound LoadWavSound(const char* filename);
LPDIRECTSOUNDBUFFER LoadWavSound_BGM(const char* filename);
void SetStage_BGM();
void sound_load();
