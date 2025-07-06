#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <thread>
#include <atomic>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
    // Note: -lwinmm is specified in Makefile instead of pragma comment
#endif

class Audio {
private:
#ifdef _WIN32
    std::string currentMusicFile;
    bool musicPlaying;
    std::thread musicThread;
    std::atomic<bool> shouldStopMusic;
#endif
    
    std::string currentMusic;
    bool isInitialized;
    
public:
    Audio();
    ~Audio();
    bool initialize();
    void update();
    void cleanup();
    void playMenuMusic();
    void playGameMusic();
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(float volume);
    float getMusicVolume();
    bool isMusicPlaying();
    std::string getCurrentMusic();
    bool isAudioWorking();
    
#ifdef _WIN32
private:
    void playMusicWindows(const std::string& filename, bool loop = true);
    void stopMusicWindows();
#endif
};

#endif // AUDIO_H