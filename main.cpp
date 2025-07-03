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

// Variáveis globais do jogo
Player* player;
Scene* scene;
Score* score;
Menu* menu;
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
const float OBSTACLE_SPAWN_INTERVAL = 2.0f;

// Função para spawnar obstáculos
void spawnObstacle() {
    // Escolher faixa aleatória (0, 1, 2)
    int lane = rand() % 3;
    float x = (lane - 1) * 3.0f; // Converter para posição X
    
    // Escolher tipo de obstáculo
    ObstacleType type = (rand() % 2 == 0) ? STATIC : MOVING_VERTICAL;
    
    // Posição inicial (à frente do jogador)
    Vector3 position(x, 1.0f, -30.0f);
    Vector3 size(1.0f, 2.0f, 1.0f);
    
    // Encontrar um obstáculo inativo para reutilizar
    bool spawned = false;
    for (auto& obstacle : obstacles) {
        if (!obstacle.isActive()) {
            obstacle.reset(position, size, type);
            spawned = true;
            break;
        }
    }
    
    // Se não encontrou um inativo, criar novo
    if (!spawned && obstacles.size() < 20) {
        obstacles.push_back(Obstacle(position, size, type));
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
    Lighting::init();
    
    // Criar objetos do jogo
    player = new Player();
    scene = new Scene();
    score = new Score();
    menu = new Menu();
    
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
            // Configurar câmera
            scene->setupCamera();
            
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
                    break;
                case 'd':
                case 'D':
                    debugMode = !debugMode;
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
                    break;
                case 27: // ESC
                    gameState = MENU;
                    menu->setState(MENU);
                    break;
            }
            break;
            
        case PAUSED:
            switch (key) {
                case 13: // Enter
                    if (menu->getSelectedOption() == 0) {
                        // Continuar
                        gameState = PLAYING;
                    } else if (menu->getSelectedOption() == 1) {
                        // Menu principal
                        gameState = MENU;
                        menu->setState(MENU);
                    }
                    break;
                case 27: // ESC
                    gameState = PLAYING;
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

// Função de limpeza
void cleanup() {
    delete player;
    delete scene;
    delete score;
    delete menu;
    
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
    std::cout << "  Espaco      : Pular" << std::endl;
    std::cout << "  ESC         : Menu/Pausar" << std::endl;
    std::cout << "  D           : Debug mode" << std::endl;
    std::cout << "===================" << std::endl;
    
    // Iniciar loop principal
    glutMainLoop();
    
    return 0;
} 