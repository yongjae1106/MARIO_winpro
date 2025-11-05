#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <map>
#include <string>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

class Sound {
public:
    Sound();
    ~Sound();

    bool init(HWND hwnd);
    void loadSound(const std::string& name, const std::wstring& filename);
    void loadAllSounds();
    void play(const std::string& name, bool loop = false);
    void stop(const std::string& name);

private:
    LPDIRECTSOUND8 m_pDS;
    std::map<std::string, LPDIRECTSOUNDBUFFER> m_soundBuffers;

    LPDIRECTSOUNDBUFFER loadWav(const std::wstring& filename);
};
