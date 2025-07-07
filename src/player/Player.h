#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include "../utils/Vector3.h"
#include <cmath>

class Player {
private:
    Vector3 position;
    Vector3 velocity;
    int currentLane;        // 0 = esquerda, 1 = centro, 2 = direita
    int targetLane;         // Faixa para onde está se movendo
    float laneTransitionTime;
    float laneTransitionSpeed;
    bool isJumping;
    bool isSliding;
    float jumpTime;
    float slideTime;
    float jumpHeight;
    float size;
    
    // Variáveis de animação do astronauta
    float animationTime;    // Tempo total de animação
    float runCycleSpeed;    // Velocidade do ciclo de corrida
    float armSwingAngle;    // Ângulo de balanço dos braços
    float legSwingAngle;    // Ângulo de balanço das pernas
    float bodyBobHeight;    // Altura do balanceio do corpo
    
    // Constantes de movimento
    static const float LANE_WIDTH;
    static const float JUMP_SPEED;
    static const float GRAVITY;
    static const float GROUND_Y;
    static const float SLIDE_DURATION;
    static const float SLIDE_HEIGHT;
    static const float LANE_TRANSITION_SPEED;
    
    // Métodos de renderização do astronauta
    void renderAstronautHead();
    void renderAstronautTorso();
    void renderAstronautArm(bool isLeft, float swingAngle);
    void renderAstronautLeg(bool isLeft, float swingAngle);
    void renderAstronautJetpack();
    
public:
    Player();
    ~Player();
    
    void update(float deltaTime);
    void render();
    void jump();
    void slide();
    void moveLeft();
    void moveRight();
    void reset();
    
    // Getters
    Vector3 getPosition() const { return position; }
    Vector3 getSize() const { 
        if (isSliding) {
            return Vector3(size * 1.5f, size * 0.5f, size * 1.2f);
        }
        return Vector3(size, size * 2.2f, size); // Astronauta é mais alto (aumentado de 1.8f)
    }
    bool getIsJumping() const { return isJumping; }
    bool getIsSliding() const { return isSliding; }
    int getCurrentLane() const { return currentLane; }
    
    // Para colisão com posição ajustada
    Vector3 getMin() const;
    Vector3 getMax() const;
};

#endif