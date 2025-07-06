#ifndef PLAYER_H
#define PLAYER_H

#include "../utils/Vector3.h"
#include <GL/glut.h>

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
    
    // Constantes de movimento
    static const float LANE_WIDTH;
    static const float JUMP_SPEED;
    static const float GRAVITY;
    static const float GROUND_Y;
    static const float SLIDE_DURATION;
    static const float SLIDE_HEIGHT;
    static const float LANE_TRANSITION_SPEED;
    
public:
    Player();
    ~Player();
    
    void update(float deltaTime);
    void render();
    void jump();
    void slide();
    void moveLeft();
    void moveRight();
    
    // Getters
    Vector3 getPosition() const { return position; }
    Vector3 getSize() const { return Vector3(size, size, size); }
    bool getIsJumping() const { return isJumping; }
    bool getIsSliding() const { return isSliding; }
    int getCurrentLane() const { return currentLane; }
    
    // Para detecção de colisão
    Vector3 getMin() const;
    Vector3 getMax() const;
    
    void reset();
};

#endif 