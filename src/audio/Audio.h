#ifndef AUDIO_H
#define AUDIO_H

#include <fmod.hpp>
#include <string>
#include <thread>
#include <atomic>

class Audio {
private:
    FMOD::System* system;
    FMOD::Channel* musicChannel;
    FMOD::Sound* fasterThanLight;
    FMOD::Sound* mistyEffect;
    std::string currentMusic;
    bool isInitialized;
    
    // Fallback para mpg123
    bool useMpg123Fallback;
    std::thread mpg123Thread;
    std::atomic<bool> shouldStopMpg123;
    std::atomic<bool> mpg123Playing;
    
    void playWithMpg123(const std::string& filename, bool loop = true);
    void stopMpg123();
    
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
};

#endif // AUDIO_H 