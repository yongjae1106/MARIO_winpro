// [중요] windows.h가 winsock.h를 포함하지 않도록 막습니다.
#define WIN32_LEAN_AND_MEAN
// [중요] winsock2.h를 가장 먼저 포함합니다.
#include <winsock2.h>

#include "Sound.h"
#include <iostream>

Sound::Sound() : m_pDS(nullptr), m_currentBGM("") {}

Sound::~Sound() {
    for (auto& pair : m_soundBuffers) 
    {
        if (pair.second) 
        {
            pair.second->Release();
        }
    }
    if (m_pDS) {
        m_pDS->Release();
    }
}

bool Sound::init(HWND hwnd) {
    if (FAILED(DirectSoundCreate8(NULL, &m_pDS, NULL))) {
        return false;
    }
    if (FAILED(m_pDS->SetCooperativeLevel(hwnd, DSSCL_PRIORITY))) {
        return false;
    }
    return true;
}

void Sound::loadSound(const std::string& name, const std::wstring& filename) {
    if (m_soundBuffers.find(name) != m_soundBuffers.end()) {
        return; // Already loaded
    }
    m_soundBuffers[name] = loadWav(filename);
}

void Sound::play(const std::string& name, bool loop) {
    // If the requested sound is a BGM
    if (name.find("Theme") != std::string::npos) {
        // If a different BGM is already playing, stop it
        if (!m_currentBGM.empty() && m_currentBGM != name) {
            stop(m_currentBGM);
        }
        m_currentBGM = name;
    }

    auto it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.end() && it->second) {
        it->second->SetCurrentPosition(0);
        it->second->Play(0, 0, loop ? DSBPLAY_LOOPING : 0);
    }
}

void Sound::stop(const std::string& name) {
    auto it = m_soundBuffers.find(name);
    if (it != m_soundBuffers.end() && it->second) {
        it->second->Stop();
    }

    if (name == m_currentBGM) {
        m_currentBGM = "";
    }
}

void Sound::stopAllSounds() {
    for (auto& pair : m_soundBuffers) {
        if (pair.second) {
            pair.second->Stop();
        }
    }
    m_currentBGM = "";
}

LPDIRECTSOUNDBUFFER Sound::loadWav(const std::wstring& filename) {
    FILE* file;
    errno_t err = _wfopen_s(&file, filename.c_str(), L"rb");
    if (err != 0) {
        MessageBoxW(NULL, (L"Could not open WAV file: " + filename).c_str(), L"Error", MB_OK);
        return nullptr;
    }

    char chunkId[4];
    fread(chunkId, 1, 4, file); // "RIFF"
    fseek(file, 4, SEEK_CUR);  // skip chunk size
    fread(chunkId, 1, 4, file); // "WAVE"

    fread(chunkId, 1, 4, file); // "fmt "
    DWORD fmtSize;
    fread(&fmtSize, 4, 1, file);

    WAVEFORMATEX wfx = {};
    fread(&wfx, 1, fmtSize, file);

    fread(chunkId, 1, 4, file); // "data"
    DWORD dataSize;
    fread(&dataSize, 4, 1, file);

    BYTE* wavData = new BYTE[dataSize];
    fread(wavData, 1, dataSize, file);
    fclose(file);

    DSBUFFERDESC desc = {};
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS;
    desc.dwBufferBytes = dataSize;
    desc.lpwfxFormat = &wfx;

    LPDIRECTSOUNDBUFFER pBuf = nullptr;
    HRESULT hr = m_pDS->CreateSoundBuffer(&desc, &pBuf, NULL);
    if (FAILED(hr)) {
        delete[] wavData;
        return nullptr;
    }

    void* pLock1 = nullptr;
    DWORD size1 = 0;
    pBuf->Lock(0, dataSize, &pLock1, &size1, NULL, NULL, 0);
    memcpy(pLock1, wavData, dataSize);
    pBuf->Unlock(pLock1, size1, NULL, 0);

    delete[] wavData;
    return pBuf;
}

void Sound::loadAllSounds() {
    loadSound("1-up", L"resource/sound/smb_1-up.wav");
    loadSound("bowserdead", L"resource/sound/smb_bowserdead.wav");
    loadSound("bowserfalls", L"resource/sound/smb_bowserfalls.wav");
    loadSound("bowserfire", L"resource/sound/smb_bowserfire.wav");
    loadSound("bump", L"resource/sound/smb_bump.wav");
    loadSound("coin", L"resource/sound/smb_coin.wav");
    loadSound("fireball", L"resource/sound/smb_fireball.wav");
    loadSound("gameover", L"resource/sound/smb_gameover.wav");
    loadSound("jump-small", L"resource/sound/smb_jump-small.wav");
    loadSound("jump-super", L"resource/sound/smb_jump-super.wav");
    loadSound("kick", L"resource/sound/smb_kick.wav");
    loadSound("mariodie", L"resource/sound/smb_mariodie.wav");
    loadSound("pipe", L"resource/sound/smb_pipe.wav");
    loadSound("powerup_appears", L"resource/sound/smb_powerup_appears.wav");
    loadSound("powerup", L"resource/sound/smb_powerup.wav");
    loadSound("stage_clear", L"resource/sound/smb_stage_clear.wav");
    loadSound("stomp", L"resource/sound/smb_stomp.wav");
    loadSound("tino_attack", L"resource/sound/smb_tino_attack.wav");
    loadSound("world_clear", L"resource/sound/smb_world_clear.wav");

    // BGM
    loadSound("GroundTheme", L"resource/sound/bgm/GroundTheme.wav");
    loadSound("CastleTheme", L"resource/sound/bgm/CastleTheme.wav");
    loadSound("InvincibilityTheme", L"resource/sound/bgm/InvincibilityTheme.wav");
}
