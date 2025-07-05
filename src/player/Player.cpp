#include "Player.h"
#include <cmath>

// Definição das constantes
const float Player::LANE_WIDTH = 3.0f;
const float Player::JUMP_SPEED = 15.0f;
const float Player::GRAVITY = -30.0f;
const float Player::GROUND_Y = 0.5f;
const float Player::SLIDE_DURATION = 0.8f;
const float Player::SLIDE_HEIGHT = 0.3f;

Player::Player() {
    position = Vector3(0.0f, GROUND_Y, 0.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);
    currentLane = 1; // Começa no centro
    isJumping = false;
    isSliding = false;
    jumpTime = 0.0f;
    slideTime = 0.0f;
    jumpHeight = 0.0f;
    size = 1.0f;
}

Player::~Player() {
}

void Player::update(float deltaTime) {
    // Atualizar posição X baseada na faixa atual
    float targetX = (currentLane - 1) * LANE_WIDTH;
    position.x = targetX;
    
    // Atualizar física do pulo
    if (isJumping) {
        jumpTime += deltaTime;
        velocity.y += GRAVITY * deltaTime;
        position.y += velocity.y * deltaTime;
        
        // Verificar se tocou o chão
        if (position.y <= GROUND_Y) {
            position.y = GROUND_Y;
            isJumping = false;
            velocity.y = 0.0f;
            jumpTime = 0.0f;
        }
    }
    
    // Atualizar física do deslize
    if (isSliding) {
        slideTime += deltaTime;
        
        // Altura do deslize diminui gradualmente
        float slideProgress = slideTime / SLIDE_DURATION;
        float slideHeight = SLIDE_HEIGHT * (1.0f - slideProgress);
        position.y = GROUND_Y - slideHeight;
        
        // Verificar se o deslize terminou
        if (slideTime >= SLIDE_DURATION) {
            position.y = GROUND_Y;
            isSliding = false;
            slideTime = 0.0f;
        }
    }
}

void Player::render() {
    glPushMatrix();
    
    // Posicionar o jogador
    glTranslatef(position.x, position.y, position.z);
    
    // Escala baseada no estado (deslize torna o jogador mais baixo e largo)
    float scaleX = size;
    float scaleY = size;
    float scaleZ = size;
    
    if (isSliding) {
        scaleX *= 1.5f;  // Mais largo
        scaleY *= 0.5f;  // Mais baixo
        scaleZ *= 1.2f;  // Um pouco mais longo
    }
    
    // Cor do jogador (azul ciano brilhante)
    glColor3f(0.0f, 0.8f, 1.0f);
    
    // Renderizar cubo principal
    glPushMatrix();
    glScalef(scaleX, scaleY, scaleZ);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Efeito de brilho interno
    glColor3f(0.5f, 1.0f, 1.0f);
    glPushMatrix();
    glScalef(scaleX * 0.8f, scaleY * 0.8f, scaleZ * 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Núcleo brilhante
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glScalef(scaleX * 0.3f, scaleY * 0.3f, scaleZ * 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Contorno dourado
    glColor3f(1.0f, 0.8f, 0.0f);
    glLineWidth(2.0f);
    glPushMatrix();
    glScalef(scaleX, scaleY, scaleZ);
    glutWireCube(1.0f);
    glPopMatrix();
    
    // Efeito de deslize (rastro de energia)
    if (isSliding) {
        glColor3f(0.0f, 1.0f, 0.5f); // Verde ciano
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        // Linhas de energia atrás do jogador
        for (int i = 0; i < 5; i++) {
            float offset = i * 0.3f;
            glVertex3f(-scaleX * 0.3f, 0.0f, -scaleZ * 0.5f - offset);
            glVertex3f(scaleX * 0.3f, 0.0f, -scaleZ * 0.5f - offset);
        }
        glEnd();
    }
    
    glPopMatrix();
}

void Player::jump() {
    if (!isJumping && !isSliding) {
        isJumping = true;
        velocity.y = JUMP_SPEED;
        jumpTime = 0.0f;
    }
}

void Player::slide() {
    if (!isSliding && !isJumping) {
        isSliding = true;
        slideTime = 0.0f;
    }
}

void Player::moveLeft() {
    if (currentLane > 0) {
        currentLane--;
    }
}

void Player::moveRight() {
    if (currentLane < 2) {
        currentLane++;
    }
}

Vector3 Player::getMin() const {
    float scaleX = size;
    float scaleY = size;
    float scaleZ = size;
    
    if (isSliding) {
        scaleX *= 1.5f;
        scaleY *= 0.5f;
        scaleZ *= 1.2f;
    }
    
    return Vector3(position.x - scaleX/2, position.y - scaleY/2, position.z - scaleZ/2);
}

Vector3 Player::getMax() const {
    float scaleX = size;
    float scaleY = size;
    float scaleZ = size;
    
    if (isSliding) {
        scaleX *= 1.5f;
        scaleY *= 0.5f;
        scaleZ *= 1.2f;
    }
    
    return Vector3(position.x + scaleX/2, position.y + scaleY/2, position.z + scaleZ/2);
}

void Player::reset() {
    position = Vector3(0.0f, GROUND_Y, 0.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);
    currentLane = 1;
    isJumping = false;
    isSliding = false;
    jumpTime = 0.0f;
    slideTime = 0.0f;
    jumpHeight = 0.0f;
} 