#include "Audio.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

Audio::Audio() : system(nullptr), musicChannel(nullptr), 
                 fasterThanLight(nullptr), mistyEffect(nullptr),
                 currentMusic(""), isInitialized(false),
                 useMpg123Fallback(false), shouldStopMpg123(false), mpg123Playing(false) {
}

Audio::~Audio() {
    cleanup();
}

bool Audio::initialize() {
    FMOD_RESULT result;
    
    // Criar sistema FMOD
    result = FMOD::System_Create(&system);
    if (result != FMOD_OK) {
        std::cerr << "Erro ao criar sistema FMOD: " << result << std::endl;
        return false;
    }
    
    // Verificar se há drivers de áudio disponíveis ANTES de inicializar
    int numDrivers;
    result = system->getNumDrivers(&numDrivers);
    if (result != FMOD_OK || numDrivers == 0) {
        std::cout << "=== AVISO: SISTEMA DE ÁUDIO INDISPONÍVEL ===" << std::endl;
        std::cout << "Nenhum driver de áudio foi encontrado." << std::endl;
        std::cout << "O jogo continuará funcionando sem áudio." << std::endl;
        
        if (result == FMOD_OK && numDrivers == 0) {
            std::cout << "\nPara habilitar áudio no WSL2:" << std::endl;
            std::cout << "1. No Windows, execute: wsl --shutdown" << std::endl;
            std::cout << "2. Execute: wsl --update" << std::endl;
            std::cout << "3. Crie/edite o arquivo C:\\Users\\%USERNAME%\\.wslconfig:" << std::endl;
            std::cout << "   [wsl2]" << std::endl;
            std::cout << "   audio=true" << std::endl;
            std::cout << "4. Reinicie o WSL" << std::endl;
            std::cout << "\nAlternativamente, instale PulseAudio:" << std::endl;
            std::cout << "   sudo apt update && sudo apt install pulseaudio" << std::endl;
        }
        std::cout << "=============================================" << std::endl;
        
        // Configurar sistema para modo NoSound para que tudo funcione
        system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        result = system->init(32, FMOD_INIT_NORMAL, nullptr);
        
        if (result != FMOD_OK) {
            std::cerr << "Erro crítico ao inicializar FMOD em modo silencioso: " << result << std::endl;
            system->release();
            system = nullptr;
            isInitialized = false;
            return true; // Ainda retorna true para o jogo continuar
        }
        
        std::cout << "FMOD inicializado em modo silencioso" << std::endl;
    } else {
        // Tentar inicialização normal com áudio
        std::cout << "FMOD encontrou " << numDrivers << " driver(s) de áudio" << std::endl;
        
        // Tentar diferentes outputs para WSL2/Linux com WSLg
        result = FMOD_ERR_OUTPUT_INIT; // Começar assumindo falha
        
        // WSL2 com WSLg usa PulseAudio através do servidor do Windows
        std::cout << "Detectado ambiente WSL2 - tentando PulseAudio..." << std::endl;
        system->setOutput(FMOD_OUTPUTTYPE_PULSEAUDIO);
        result = system->init(32, FMOD_INIT_NORMAL, nullptr);
        
        if (result != FMOD_OK) {
            std::cout << "PulseAudio falhou (" << result << "), tentando ALSA..." << std::endl;
            system->close();
            system->setOutput(FMOD_OUTPUTTYPE_ALSA);
            result = system->init(32, FMOD_INIT_NORMAL, nullptr);
        }
        
        if (result != FMOD_OK) {
            std::cout << "ALSA falhou (" << result << "), tentando configuração manual..." << std::endl;
            system->close();
            
            // Tentar com configurações específicas para WSL2
            system->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            result = system->init(32, FMOD_INIT_NORMAL, nullptr);
        }
        
        if (result != FMOD_OK) {
            std::cout << "Autodetect falhou (" << result << "), usando modo silencioso..." << std::endl;
            system->close();
            
            // Última tentativa: modo NoSound para pelo menos o jogo funcionar
            system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
            result = system->init(32, FMOD_INIT_NORMAL, nullptr);
            
            if (result == FMOD_OK) {
                std::cout << "AVISO: Funcionando em modo silencioso (sem áudio)" << std::endl;
                std::cout << "Para habilitar áudio no WSL2:" << std::endl;
                std::cout << "1. Certifique-se de que o WSLg está instalado" << std::endl;
                std::cout << "2. Configure as variáveis de ambiente:" << std::endl;
                std::cout << "   export PULSE_RUNTIME_PATH=/mnt/wslg/runtime-dir/pulse" << std::endl;
                std::cout << "   export PULSE_SERVER=unix:/mnt/wslg/PulseServer" << std::endl;
                
                // Verificar se mpg123 está disponível como fallback
                if (std::system("which mpg123 >/dev/null 2>&1") == 0) {
                    std::cout << "\nmpg123 detectado - tentando usar como fallback para áudio..." << std::endl;
                    useMpg123Fallback = true;
                }
            } else {
                std::cerr << "Erro crítico ao inicializar FMOD: " << result << std::endl;
                system->release();
                system = nullptr;
                isInitialized = false;
                return true; // Retorna true para o jogo continuar funcionando
            }
        }
    }
    
    // Carregar músicas (funciona mesmo em modo silencioso)
    result = system->createSound("src/sounds/faster_than_light.mp3", FMOD_LOOP_NORMAL, nullptr, &fasterThanLight);
    if (result != FMOD_OK) {
        std::cerr << "Erro ao carregar faster_than_light.mp3: " << result << std::endl;
        // Não retorna false - continua sem esta música
    } else {
        std::cout << "Música faster_than_light.mp3 carregada com sucesso!" << std::endl;
    }
    
    result = system->createSound("src/sounds/misty_effect.mp3", FMOD_LOOP_NORMAL, nullptr, &mistyEffect);
    if (result != FMOD_OK) {
        std::cerr << "Erro ao carregar misty_effect.mp3: " << result << std::endl;
        // Não retorna false - continua sem esta música
    } else {
        std::cout << "Música misty_effect.mp3 carregada com sucesso!" << std::endl;
    }
    
    isInitialized = true;
    
    // Verificar qual output está sendo usado
    FMOD_OUTPUTTYPE outputType;
    system->getOutput(&outputType);
    std::cout << "Sistema de áudio FMOD inicializado com sucesso!" << std::endl;
    std::cout << "Tipo de saída de áudio: " << (outputType == FMOD_OUTPUTTYPE_NOSOUND ? "NoSound (Silencioso)" : "Hardware") << std::endl;
    
    return true;
}

void Audio::update() {
    if (isInitialized && system) {
        system->update();
    }
}

void Audio::cleanup() {
    // Parar mpg123 se estiver rodando
    stopMpg123();
    
    if (fasterThanLight) {
        fasterThanLight->release();
        fasterThanLight = nullptr;
    }
    
    if (mistyEffect) {
        mistyEffect->release();
        mistyEffect = nullptr;
    }
    
    if (system) {
        system->close();
        system->release();
        system = nullptr;
    }
    
    isInitialized = false;
}

void Audio::playMenuMusic() {
    if (!isInitialized) {
        return;
    }
    
    // Se estamos usando mpg123 como fallback
    if (useMpg123Fallback) {
        stopMpg123(); // Parar música anterior
        playWithMpg123("src/sounds/faster_than_light.mp3", true);
        currentMusic = "faster_than_light";
        std::cout << "Tocando música do menu com mpg123: faster_than_light" << std::endl;
        return;
    }
    
    // Usar FMOD normalmente
    if (!system || !fasterThanLight) {
        return;
    }
    
    // Parar música atual se estiver tocando
    if (musicChannel) {
        musicChannel->stop();
    }
    
    // Tocar música do menu
    FMOD_RESULT result = system->playSound(fasterThanLight, nullptr, false, &musicChannel);
    if (result == FMOD_OK) {
        currentMusic = "faster_than_light";
        // Definir volume para 100%
        musicChannel->setVolume(1.0f);
        std::cout << "Tocando música do menu: faster_than_light (volume: 100%)" << std::endl;
    } else {
        std::cerr << "Erro ao tocar música do menu: " << result << std::endl;
    }
}

void Audio::playGameMusic() {
    if (!isInitialized) {
        return;
    }
    
    // Se estamos usando mpg123 como fallback
    if (useMpg123Fallback) {
        stopMpg123(); // Parar música anterior
        playWithMpg123("src/sounds/misty_effect.mp3", true);
        currentMusic = "misty_effect";
        std::cout << "Tocando música do jogo com mpg123: misty_effect" << std::endl;
        return;
    }
    
    // Usar FMOD normalmente
    if (!system || !mistyEffect) {
        return;
    }
    
    // Parar música atual se estiver tocando
    if (musicChannel) {
        musicChannel->stop();
    }
    
    // Tocar música do jogo
    FMOD_RESULT result = system->playSound(mistyEffect, nullptr, false, &musicChannel);
    if (result == FMOD_OK) {
        currentMusic = "misty_effect";
        // Definir volume para 100%
        musicChannel->setVolume(1.0f);
        std::cout << "Tocando música do jogo: misty_effect (volume: 100%)" << std::endl;
    } else {
        std::cerr << "Erro ao tocar música do jogo: " << result << std::endl;
    }
}

void Audio::stopMusic() {
    if (useMpg123Fallback) {
        stopMpg123();
        currentMusic = "";
        return;
    }
    
    if (musicChannel) {
        musicChannel->stop();
        currentMusic = "";
        std::cout << "Música parada" << std::endl;
    }
}

void Audio::pauseMusic() {
    if (musicChannel) {
        musicChannel->setPaused(true);
        std::cout << "Música pausada" << std::endl;
    }
}

void Audio::resumeMusic() {
    if (musicChannel) {
        musicChannel->setPaused(false);
        std::cout << "Música resumida" << std::endl;
    }
}

void Audio::setMusicVolume(float volume) {
    if (musicChannel) {
        // Volume deve estar entre 0.0 e 1.0
        volume = std::max(0.0f, std::min(1.0f, volume));
        musicChannel->setVolume(volume);
    }
}

float Audio::getMusicVolume() {
    float volume = 0.0f;
    if (musicChannel) {
        musicChannel->getVolume(&volume);
    }
    return volume;
}

bool Audio::isMusicPlaying() {
    bool playing = false;
    if (musicChannel) {
        musicChannel->isPlaying(&playing);
    }
    return playing;
}

std::string Audio::getCurrentMusic() {
    return currentMusic;
}

bool Audio::isAudioWorking() {
    if (!isInitialized || !system) {
        return false;
    }
    
    int numDrivers;
    FMOD_RESULT result = system->getNumDrivers(&numDrivers);
    return (result == FMOD_OK && numDrivers > 0);
}

void Audio::playWithMpg123(const std::string& filename, bool loop) {
    // Parar qualquer reprodução anterior
    stopMpg123();
    
    if (mpg123Thread.joinable()) {
        mpg123Thread.join();
    }
    
    shouldStopMpg123 = false;
    mpg123Playing = true;
    
    // Criar thread para reprodução
    mpg123Thread = std::thread([this, filename, loop]() {
        std::string command;
        if (loop) {
            // Reproduzir em loop infinito
            command = "while [ ! -f /tmp/stop_mpg123_" + std::to_string(getpid()) + " ]; do mpg123 -q \"" + filename + "\" 2>/dev/null; sleep 0.1; done";
        } else {
            command = "mpg123 -q \"" + filename + "\" 2>/dev/null";
        }
        
        std::cout << "Iniciando reprodução com mpg123: " << filename << std::endl;
        std::system(command.c_str());
        mpg123Playing = false;
    });
}

void Audio::stopMpg123() {
    if (mpg123Playing) {
        shouldStopMpg123 = true;
        // Criar arquivo de sinal para parar o loop
        std::string stopFile = "/tmp/stop_mpg123_" + std::to_string(getpid());
        std::system(("touch " + stopFile).c_str());
        
        // Matar processos mpg123 se estiverem rodando
        std::system("pkill -f mpg123 2>/dev/null");
        
        if (mpg123Thread.joinable()) {
            mpg123Thread.join();
        }
        
        // Limpar arquivo de sinal
        std::system(("rm -f " + stopFile).c_str());
        mpg123Playing = false;
        std::cout << "Reprodução mpg123 parada" << std::endl;
    }
}