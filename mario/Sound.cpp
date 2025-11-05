#include "Sound.h"
#include <iostream>

Sound::Sound() : m_pDS(nullptr) {}

Sound::~Sound() {
    for (auto& pair : m_soundBuffers) {
        if (pair.second) {
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
}

LPDIRECTSOUNDBUFFER Sound::loadWav(const std::wstring& filename) {
    FILE* file = _wfopen(filename.c_str(), L"rb");
    if (!file) {
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
}
