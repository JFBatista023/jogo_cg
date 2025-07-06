#include "Audio.h"
#include <iostream>
#include <fstream>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
    #include <thread>
    #include <chrono>
#endif

Audio::Audio() : currentMusic(""), isInitialized(false) {
#ifdef _WIN32
    musicPlaying = false;
    shouldStopMusic = false;
#endif
}

Audio::~Audio() {
    cleanup();
}

bool Audio::initialize() {
    std::cout << "=== INICIALIZANDO SISTEMA DE ÁUDIO WINDOWS ===" << std::endl;
    
#ifdef _WIN32
    std::cout << "Usando Windows Multimedia API..." << std::endl;
    isInitialized = true;
    std::cout << "Sistema de áudio Windows inicializado!" << std::endl;
    
    // Verificar se os arquivos de música existem
    std::ifstream menuFile("src/sounds/faster_than_light.wav");
    std::ifstream gameFile("src/sounds/misty_effect.wav");
    
    if (menuFile.good()) {
        std::cout << "✓ Música do menu encontrada: faster_than_light.wav" << std::endl;
    } else {
        std::cout << "✗ Música do menu não encontrada: src/sounds/faster_than_light.wav" << std::endl;
    }
    
    if (gameFile.good()) {
        std::cout << "✓ Música do jogo encontrada: misty_effect.wav" << std::endl;
    } else {
        std::cout << "✗ Música do jogo não encontrada: src/sounds/misty_effect.wav" << std::endl;
    }
    
    return true;
#else
    std::cout << "Sistema não Windows detectado!" << std::endl;
    return false;
#endif
}

void Audio::update() {
    // Não precisa de update manual no Windows
}

void Audio::cleanup() {
#ifdef _WIN32
    stopMusicWindows();
    if (musicThread.joinable()) {
        musicThread.join();
    }
#endif
    isInitialized = false;
}

void Audio::playMenuMusic() {
    if (!isInitialized) return;
    
    // Parar música atual se estiver tocando
    stopMusic();
    
    currentMusic = "faster_than_light";
    std::cout << "Iniciando música do menu..." << std::endl;
    
#ifdef _WIN32
    // Lista de possíveis localizações para o arquivo de música do menu
    std::vector<std::string> musicFiles = {
        "src/sounds/faster_than_light.wav",
        "sounds/faster_than_light.wav",
        "faster_than_light.wav"
    };
    
    for (const auto& file : musicFiles) {
        std::ifstream test(file);
        if (test.good()) {
            playMusicWindows(file, true);
            std::cout << "♪ Tocando música do menu: " << file << std::endl;
            return;
        }
    }
    std::cout << "⚠ Nenhum arquivo de música do menu encontrado!" << std::endl;
    std::cout << "  Procurado: faster_than_light.wav" << std::endl;
#endif
}

void Audio::playGameMusic() {
    if (!isInitialized) return;
    
    // Parar música atual se estiver tocando
    stopMusic();
    
    currentMusic = "misty_effect";
    std::cout << "Iniciando música do jogo..." << std::endl;
    
#ifdef _WIN32
    // Lista de possíveis localizações para o arquivo de música do jogo
    std::vector<std::string> musicFiles = {
        "src/sounds/misty_effect.wav",
        "sounds/misty_effect.wav",
        "misty_effect.wav"
    };
    
    for (const auto& file : musicFiles) {
        std::ifstream test(file);
        if (test.good()) {
            playMusicWindows(file, true);
            std::cout << "♪ Tocando música do jogo: " << file << std::endl;
            return;
        }
    }
    std::cout << "⚠ Nenhum arquivo de música do jogo encontrado!" << std::endl;
    std::cout << "  Procurado: misty_effect.wav" << std::endl;
#endif
}

void Audio::stopMusic() {
    if (!isInitialized) return;
    
#ifdef _WIN32
    stopMusicWindows();
#endif
    
    if (!currentMusic.empty()) {
        std::cout << "⏹ Parando música: " << currentMusic << std::endl;
        currentMusic = "";
    }
}

void Audio::pauseMusic() {
    if (!isInitialized) return;
    
#ifdef _WIN32
    if (musicPlaying) {
        mciSendStringA("pause music", NULL, 0, NULL);
        std::cout << "⏸ Música pausada: " << currentMusic << std::endl;
    }
#endif
}

void Audio::resumeMusic() {
    if (!isInitialized) return;
    
#ifdef _WIN32
    if (musicPlaying && !currentMusic.empty()) {
        mciSendStringA("resume music", NULL, 0, NULL);
        std::cout << "▶ Música retomada: " << currentMusic << std::endl;
    } else {
        // Reiniciar música se não estiver tocando
        std::cout << "🔄 Reiniciando música: " << currentMusic << std::endl;
        if (currentMusic == "faster_than_light") {
            playMenuMusic();
        } else if (currentMusic == "misty_effect") {
            playGameMusic();
        }
    }
#endif
}

void Audio::setMusicVolume(float volume) {
    if (!isInitialized) return;
    
#ifdef _WIN32
    // Limitar volume entre 0.0 e 1.0
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    
    DWORD vol = (DWORD)(volume * 0xFFFF);
    waveOutSetVolume(0, MAKELONG(vol, vol));
    std::cout << "🔊 Volume ajustado para: " << (int)(volume * 100) << "%" << std::endl;
#endif
}

float Audio::getMusicVolume() {
    if (!isInitialized) return 1.0f;
    
#ifdef _WIN32
    DWORD volume;
    if (waveOutGetVolume(0, &volume) == MMSYSERR_NOERROR) {
        return (float)(LOWORD(volume)) / 0xFFFF;
    }
#endif
    
    return 1.0f;
}

bool Audio::isMusicPlaying() {
#ifdef _WIN32
    return musicPlaying;
#endif
    return false;
}

std::string Audio::getCurrentMusic() {
    return currentMusic;
}

bool Audio::isAudioWorking() {
    return isInitialized;
}

#ifdef _WIN32
void Audio::playMusicWindows(const std::string& filename, bool loop) {
    // Parar música atual primeiro
    stopMusicWindows();
    
    currentMusicFile = filename;
    shouldStopMusic = false;
    musicPlaying = true;
    
    // Criar thread para tocar música
    musicThread = std::thread([this, filename, loop]() {
        std::cout << "🎵 Iniciando reprodução: " << filename << std::endl;
        
        // Abrir arquivo de áudio usando MCI
        std::string openCommand = "open \"" + filename + "\" type waveaudio alias music";
        MCIERROR openResult = mciSendStringA(openCommand.c_str(), NULL, 0, NULL);
        
        if (openResult != 0) {
            char errorBuffer[128];
            mciGetErrorStringA(openResult, errorBuffer, sizeof(errorBuffer));
            std::cout << "❌ Erro ao abrir arquivo: " << errorBuffer << std::endl;
            musicPlaying = false;
            return;
        }
        
        std::cout << "✓ Arquivo de áudio aberto com sucesso" << std::endl;
        
        do {
            if (shouldStopMusic) break;
            
            // Tocar música do início
            MCIERROR playResult = mciSendStringA("play music from 0", NULL, 0, NULL);
            if (playResult != 0) {
                char errorBuffer[128];
                mciGetErrorStringA(playResult, errorBuffer, sizeof(errorBuffer));
                std::cout << "❌ Erro ao reproduzir: " << errorBuffer << std::endl;
                break;
            }
            
            // Aguardar término da reprodução
            char status[32];
            do {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
                if (shouldStopMusic) break;
                
                MCIERROR statusResult = mciSendStringA("status music mode", status, 32, NULL);
                if (statusResult != 0) {
                    std::cout << "❌ Erro ao verificar status" << std::endl;
                    break;
                }
                
            } while (strcmp(status, "stopped") != 0 && !shouldStopMusic);
            
            if (shouldStopMusic) break;
            
            // Se chegou aqui e não deve parar, a música terminou naturalmente
            if (loop) {
                std::cout << "🔄 Reiniciando música em loop..." << std::endl;
            }
            
        } while (loop && !shouldStopMusic);
        
        // Fechar arquivo
        mciSendStringA("close music", NULL, 0, NULL);
        musicPlaying = false;
        
        std::cout << "⏹ Reprodução finalizada: " << filename << std::endl;
    });
}

void Audio::stopMusicWindows() {
    if (musicPlaying) {
        shouldStopMusic = true;
        
        // Parar reprodução imediatamente
        mciSendStringA("stop music", NULL, 0, NULL);
        mciSendStringA("close music", NULL, 0, NULL);
        
        // Aguardar thread terminar
        if (musicThread.joinable()) {
            musicThread.join();
        }
        
        musicPlaying = false;
    }
}
#endif