#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include "src/player/Player.h"
#include "src/obstacle/Obstacle.h"
#include "src/scene/Scene.h"
#include "src/collision/Collision.h"
#include "src/score/Score.h"
#include "src/menu/Menu.h"
#include "src/texture/Texture.h"
#include "src/lighting/Lighting.h"
#include "src/audio/Audio.h"

// Variáveis globais do jogo
Player* player;
Scene* scene;
Score* score;
Menu* menu;
Audio* audio;
std::vector<Obstacle> obstacles;

// Controle de tempo
std::chrono::high_resolution_clock::time_point lastTime;
float deltaTime = 0.0f;

// Configurações da janela
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Controle do jogo
GameState gameState = MENU;
bool debugMode = false;
float obstacleSpawnTimer = 0.0f;
float OBSTACLE_SPAWN_INTERVAL = 2.0f;

// Sistema de velocidade progressiva
float gameTime = 0.0f;
float speedMultiplier = 1.0f;
const float SPEED_INCREASE_INTERVAL = 20.0f; // A cada 20 segundos
const float SPEED_INCREASE_AMOUNT = 0.5f; // Aumenta 0.5x

// Sistema de câmera
bool firstPersonView = false;

// Declarações de funções
void spawnObstacle();
void spawnSingleObstacle();
void spawnTwoObstacles();
void setupFirstPersonCamera();

// Função para spawnar obstáculos
void spawnObstacle() {
    // Decidir se vai spawnar 1 ou 2 obstáculos (30% de chance para 2 obstáculos)
    bool spawnTwo = (rand() % 100) < 30;
    
    if (spawnTwo) {
        // Spawnar 2 obstáculos em faixas diferentes
        spawnTwoObstacles();
    } else {
        // Spawnar 1 obstáculo (comportamento original)
        spawnSingleObstacle();
    }
}

// Função para spawnar um único obstáculo
void spawnSingleObstacle() {
    // Escolher faixa aleatória (0, 1, 2)
    int lane = rand() % 3;
    float x = (lane - 1) * 3.0f; // Converter para posição X
    
    // Escolher tipo de obstáculo (agora com 3 tipos)
    int obstacleChoice = rand() % 3;
    ObstacleType type;
    Vector3 size;
    
    if (obstacleChoice == 0) {
        type = STATIC;
        size = Vector3(1.0f, 2.0f, 1.0f);
    } else if (obstacleChoice == 1) {
        type = MOVING_VERTICAL;
        size = Vector3(1.0f, 2.0f, 1.0f);
    } else if (obstacleChoice == 2) {
        type = ROCKET;
        size = Vector3(1.2f, 4.0f, 3.0f); // Foguete muito alto - impossível de pular
    } else {
        type = HIGH_OBSTACLE;
        size = Vector3(1.0f, 2.5f, 1.0f); // Obstáculo alto - elevado do chão
    }
    
    // Posição inicial (muito mais à frente do jogador)
    Vector3 position(x, 2.0f, -50.0f);
    
    // Ajustar posição Y para obstáculos altos (elevados do chão)
    if (type == HIGH_OBSTACLE) {
        position.y = 3.0f; // Elevado do chão para permitir deslize embaixo
    }
    
    // Encontrar um obstáculo inativo para reutilizar
    bool spawned = false;
    for (auto& obstacle : obstacles) {
        if (!obstacle.isActive()) {
            obstacle.reset(position, size, type);
            spawned = true;
            break;
        }
    }
    
    // Se não encontrou um inativo, criar novo (limite reduzido para performance)
    if (!spawned && obstacles.size() < 15) {
        obstacles.push_back(Obstacle(position, size, type));
    }
}

// Função para spawnar dois obstáculos em faixas diferentes
void spawnTwoObstacles() {
    // Escolher duas faixas diferentes
    int lane1 = rand() % 3;
    int lane2;
    do {
        lane2 = rand() % 3;
    } while (lane2 == lane1); // Garantir que são faixas diferentes
    
    float x1 = (lane1 - 1) * 3.0f;
    float x2 = (lane2 - 1) * 3.0f;
    
    // Escolher tipos de obstáculos (evitar dois obstáculos impossíveis simultaneamente)
    int choice1 = rand() % 4;
    int choice2 = rand() % 4;
    
    // Se ambos forem impossíveis de pular (ROCKET ou HIGH_OBSTACLE), mudar um deles
    if ((choice1 == 2 || choice1 == 3) && (choice2 == 2 || choice2 == 3)) {
        choice2 = rand() % 2; // Apenas STATIC ou MOVING_VERTICAL
    }
    
    // Criar primeiro obstáculo
    ObstacleType type1;
    Vector3 size1;
    
    if (choice1 == 0) {
        type1 = STATIC;
        size1 = Vector3(1.0f, 2.0f, 1.0f);
    } else if (choice1 == 1) {
        type1 = MOVING_VERTICAL;
        size1 = Vector3(1.0f, 2.0f, 1.0f);
    } else if (choice1 == 2) {
        type1 = ROCKET;
        size1 = Vector3(1.2f, 4.0f, 3.0f);
    } else {
        type1 = HIGH_OBSTACLE;
        size1 = Vector3(1.0f, 2.5f, 1.0f);
    }
    
    // Criar segundo obstáculo
    ObstacleType type2;
    Vector3 size2;
    
    if (choice2 == 0) {
        type2 = STATIC;
        size2 = Vector3(1.0f, 2.0f, 1.0f);
    } else if (choice2 == 1) {
        type2 = MOVING_VERTICAL;
        size2 = Vector3(1.0f, 2.0f, 1.0f);
    } else if (choice2 == 2) {
        type2 = ROCKET;
        size2 = Vector3(1.2f, 4.0f, 3.0f);
    } else {
        type2 = HIGH_OBSTACLE;
        size2 = Vector3(1.0f, 2.5f, 1.0f);
    }
    
    // Posições iniciais (muito mais à frente do jogador)
    Vector3 position1(x1, 2.0f, -50.0f);
    Vector3 position2(x2, 2.0f, -50.0f);
    
    // Ajustar posição Y para obstáculos altos (elevados do chão)
    if (type1 == HIGH_OBSTACLE) {
        position1.y = 3.0f;
    }
    if (type2 == HIGH_OBSTACLE) {
        position2.y = 3.0f;
    }
    
    // Encontrar obstáculos inativos para reutilizar
    int spawned = 0;
    for (auto& obstacle : obstacles) {
        if (!obstacle.isActive() && spawned < 2) {
            if (spawned == 0) {
                obstacle.reset(position1, size1, type1);
            } else {
                obstacle.reset(position2, size2, type2);
            }
            spawned++;
        }
    }
    
    // Se não encontrou inativos suficientes, criar novos (limite reduzido para performance)
    while (spawned < 2 && obstacles.size() < 15) {
        if (spawned == 0) {
            obstacles.push_back(Obstacle(position1, size1, type1));
        } else {
            obstacles.push_back(Obstacle(position2, size2, type2));
        }
        spawned++;
    }
}

// Função de inicialização
void init() {
    // Configurar OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Inicializar sistemas
    Texture::init();
    Lighting::initGameLighting();  // Usar a nova função de iluminação específica do jogo
    
    // Criar objetos do jogo
    player = new Player();
    scene = new Scene();
    score = new Score();
    menu = new Menu();
    audio = new Audio();
    
    // Inicializar sistema de áudio
    if (!audio->initialize()) {
        std::cerr << "Aviso: Sistema de áudio não pôde ser inicializado!" << std::endl;
    } else {
        // Tocar música do menu inicial
        audio->playMenuMusic();
    }
    
    // Inicializar cena
    scene->init();
    
    // Configurar tempo inicial
    lastTime = std::chrono::high_resolution_clock::now();
    
    // Inicializar gerador de números aleatórios
    srand(time(NULL));
    
    std::cout << "Cosmic Dash inicializado com sucesso!" << std::endl;
}

// Função de atualização
void update() {
    // Calcular delta time
    auto currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    // Limitar delta time para evitar grandes saltos
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    
    // Atualizar sistema de áudio
    if (audio) {
        audio->update();
    }
    
    // Atualizar tempo de jogo e velocidade (para todos os estados)
    if (gameState == PLAYING) {
        gameTime += deltaTime;
        
        // Aumentar velocidade a cada 20 segundos
        int currentSpeedLevel = (int)(gameTime / SPEED_INCREASE_INTERVAL);
        speedMultiplier = 1.0f + (currentSpeedLevel * SPEED_INCREASE_AMOUNT);
        
        // Atualizar velocidade da cena
        scene->setFloorSpeed(20.0f * speedMultiplier);
        
        // Ajustar intervalo de spawn baseado na velocidade (mais rápido = menos obstáculos por segundo)
        OBSTACLE_SPAWN_INTERVAL = 2.0f + (speedMultiplier - 1.0f) * 0.3f;
    }
    
    switch (gameState) {
        case PLAYING:
            // Atualizar jogador
            player->update(deltaTime);
            
            // Atualizar cena
            scene->update(deltaTime);
            
            // Atualizar pontuação
            score->update(deltaTime);
            
            // Spawnar obstáculos
            obstacleSpawnTimer += deltaTime;
            if (obstacleSpawnTimer >= OBSTACLE_SPAWN_INTERVAL) {
                spawnObstacle();
                obstacleSpawnTimer = 0.0f;
            }
            
            // Atualizar obstáculos
            for (auto& obstacle : obstacles) {
                if (obstacle.isActive()) {
                    obstacle.update(deltaTime);
                    obstacle.moveZ(scene->getFloorSpeed() * deltaTime);
                    
                    // Desativar obstáculos que saíram da tela
                    if (obstacle.getPosition().z > 20.0f) {
                        obstacle.setActive(false);
                    }
                }
            }
            
            // Verificar colisões
            if (Collision::checkCollisionWithObstacles(*player, obstacles)) {
                gameState = GAME_OVER;
                score->gameOver();
                menu->setState(GAME_OVER);
            }
            
            break;
            
        case MENU:
        case GAME_OVER:
        case PAUSED:
            // Estados de menu não precisam de atualização de jogo
            break;
    }
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Configurar projeção 3D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 1000.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    switch (gameState) {
        case PLAYING:
            // Configurar câmera baseada no modo de visão
            if (firstPersonView) {
                setupFirstPersonCamera();
            } else {
                scene->setupCamera();
            }
            
            // Renderizar cena
            scene->render();
            
            // Renderizar jogador
            player->render();
            
            // Renderizar obstáculos
            for (const auto& obstacle : obstacles) {
                obstacle.render();
            }
            
            // Debug: renderizar bounding boxes
            if (debugMode) {
                Collision::renderPlayerBoundingBox(*player);
                for (const auto& obstacle : obstacles) {
                    Collision::renderObstacleBoundingBox(obstacle);
                }
            }
            
            // Renderizar HUD
            score->render();
            break;
            
        case MENU:
            menu->render();
            break;
            
        case GAME_OVER:
            // Renderizar cena em segundo plano (sem atualização)
            scene->setupCamera();
            scene->render();
            player->render();
            for (const auto& obstacle : obstacles) {
                obstacle.render();
            }
            
            // Renderizar tela de game over
            score->renderGameOverScreen();
            break;
            
        case PAUSED:
            // Renderizar cena pausada
            scene->setupCamera();
            scene->render();
            player->render();
            for (const auto& obstacle : obstacles) {
                obstacle.render();
            }
            
            // Renderizar menu de pausa
            menu->render();
            break;
    }
    
    glutSwapBuffers();
}

// Função de redimensionamento
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / height, 0.1, 1000.0);
    
    glMatrixMode(GL_MODELVIEW);
}

// Função de teclado
void keyboard(unsigned char key, int, int) {
    switch (gameState) {
        case PLAYING:
            switch (key) {
                case ' ':
                    player->jump();
                    break;
                case 27: // ESC
                    gameState = PAUSED;
                    menu->setState(PAUSED);
                    // Pausar música da gameplay
                    if (audio && audio->getCurrentMusic() == "misty_effect") {
                        audio->pauseMusic();
                    }
                    break;
                case 'd':
                case 'D':
                    debugMode = !debugMode;
                    break;
                case 'v':
                case 'V':
                    firstPersonView = !firstPersonView;
                    break;
            }
            break;
        
        case MENU:
            switch (key) {
                case 13: // Enter
                    if (menu->getSelectedOption() == 0) {
                        // Jogar
                        gameState = PLAYING;
                        score->startGame();
                        player->reset();
                        obstacles.clear();
                        obstacleSpawnTimer = 0.0f;
                        gameTime = 0.0f;
                        speedMultiplier = 1.0f;
                        // Parar música do menu antes de iniciar a do jogo
                        if (audio && audio->getCurrentMusic() == "faster_than_light") {
                            audio->stopMusic();
                        }
                        // Tocar música do jogo
                        if (audio) {
                            audio->playGameMusic();
                        }
                    } else if (menu->getSelectedOption() == 1) {
                        // Sair
                        exit(0);
                    }
                    break;
                case 27: // ESC
                    exit(0);
                    break;
            }
            break;
        
        case GAME_OVER:
            switch (key) {
                case 'r':
                case 'R':
                    // Reiniciar jogo
                    gameState = PLAYING;
                    score->startGame();
                    player->reset();
                    obstacles.clear();
                    obstacleSpawnTimer = 0.0f;
                    gameTime = 0.0f;
                    speedMultiplier = 1.0f;
                    // Parar música do menu antes de iniciar a do jogo
                    if (audio && audio->getCurrentMusic() == "faster_than_light") {
                        audio->stopMusic();
                    }
                    // Tocar música do jogo
                    if (audio) {
                        audio->playGameMusic();
                    }
                    break;
                case 27: // ESC
                    gameState = MENU;
                    menu->setState(MENU);
                    // Parar música da gameplay antes de iniciar a do menu
                    if (audio && audio->getCurrentMusic() == "misty_effect") {
                        audio->stopMusic();
                    }
                    // Tocar música do menu
                    if (audio) {
                        audio->playMenuMusic();
                    }
                    break;
            }
            break;
        
        case PAUSED:
            switch (key) {
                case 13: // Enter
                    if (menu->getSelectedOption() == 0) {
                        // Continuar
                        gameState = PLAYING;
                        // Retomar música da gameplay
                        if (audio && audio->getCurrentMusic() == "misty_effect") {
                            audio->resumeMusic();
                        }
                    } else if (menu->getSelectedOption() == 1) {
                        // Menu principal
                        gameState = MENU;
                        menu->setState(MENU);
                        // Parar música da gameplay antes de iniciar a do menu
                        if (audio && audio->getCurrentMusic() == "misty_effect") {
                            audio->stopMusic();
                        }
                        // Tocar música do menu
                        if (audio) {
                            audio->playMenuMusic();
                        }
                    }
                    break;
                case 27: // ESC
                    gameState = PLAYING;
                    // Retomar música da gameplay
                    if (audio && audio->getCurrentMusic() == "misty_effect") {
                        audio->resumeMusic();
                    }
                    break;
            }
            break;
    }
}

// Função de teclas especiais
void specialKeys(int key, int, int) {
    switch (gameState) {
        case PLAYING:
            switch (key) {
                case GLUT_KEY_LEFT:
                    player->moveLeft();
                    break;
                case GLUT_KEY_RIGHT:
                    player->moveRight();
                    break;
                case GLUT_KEY_DOWN:
                    player->slide();
                    break;
            }
            break;
            
        case MENU:
        case PAUSED:
            switch (key) {
                case GLUT_KEY_UP:
                    menu->navigateUp();
                    break;
                case GLUT_KEY_DOWN:
                    menu->navigateDown();
                    break;
            }
            break;
            
        case GAME_OVER:
            // Não há teclas especiais no game over
            break;
    }
}

// Função de timer para animação
void timer(int) {
    update();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Função de configuração da câmera em primeira pessoa
void setupFirstPersonCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Posição da câmera na posição do jogador
    Vector3 playerPos = player->getPosition();
    float cameraHeight = 1.5f; // Altura da câmera acima do jogador
    
    // Configurar câmera em primeira pessoa
    gluLookAt(
        playerPos.x, playerPos.y + cameraHeight, playerPos.z + 2.0f, // Posição da câmera
        playerPos.x, playerPos.y + cameraHeight, playerPos.z - 10.0f, // Ponto para onde olha
        0.0f, 1.0f, 0.0f // Vetor up
    );
}

// Função de limpeza
void cleanup() {
    delete player;
    delete scene;
    delete score;
    delete menu;
    delete audio;
    
    Texture::cleanup();
    
    std::cout << "Cosmic Dash finalizado!" << std::endl;
}

// Função principal
int main(int argc, char** argv) {
    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cosmic Dash - Endless Runner Espacial");
    
    // Registrar callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);
    
    // Inicializar jogo
    init();
    
    // Registrar função de limpeza
    atexit(cleanup);
    
    std::cout << "=== COSMIC DASH ===" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "  Setas <- -> : Mover entre faixas" << std::endl;
    std::cout << "  Seta ↓      : Deslizar" << std::endl;
    std::cout << "  Espaco      : Pular" << std::endl;
    std::cout << "  V           : Alternar visão (1ª/3ª pessoa)" << std::endl;
    std::cout << "  ESC         : Menu/Pausar" << std::endl;
    std::cout << "  D           : Debug mode" << std::endl;
    std::cout << "===================" << std::endl;
    
    // Iniciar loop principal
    glutMainLoop();
    
    return 0;
} 