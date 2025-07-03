#include "Player.h"
#include <cmath>

// Definição das constantes
const float Player::LANE_WIDTH = 3.0f;
const float Player::JUMP_SPEED = 15.0f;
const float Player::GRAVITY = -30.0f;
const float Player::GROUND_Y = 0.5f;

Player::Player() {
    position = Vector3(0.0f, GROUND_Y, 0.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);
    currentLane = 1; // Começa no centro
    isJumping = false;
    jumpTime = 0.0f;
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
}

void Player::render() {
    glPushMatrix();
    
    // Posicionar o jogador
    glTranslatef(position.x, position.y, position.z);
    
    // Cor do jogador (azul ciano brilhante)
    glColor3f(0.0f, 0.8f, 1.0f);
    
    // Renderizar cubo principal
    glPushMatrix();
    glScalef(size, size, size);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Efeito de brilho interno
    glColor3f(0.5f, 1.0f, 1.0f);
    glPushMatrix();
    glScalef(size * 0.8f, size * 0.8f, size * 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Núcleo brilhante
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glScalef(size * 0.3f, size * 0.3f, size * 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Contorno dourado
    glColor3f(1.0f, 0.8f, 0.0f);
    glLineWidth(2.0f);
    glPushMatrix();
    glScalef(size, size, size);
    glutWireCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

void Player::jump() {
    if (!isJumping) {
        isJumping = true;
        velocity.y = JUMP_SPEED;
        jumpTime = 0.0f;
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
    return Vector3(position.x - size/2, position.y - size/2, position.z - size/2);
}

Vector3 Player::getMax() const {
    return Vector3(position.x + size/2, position.y + size/2, position.z + size/2);
}

void Player::reset() {
    position = Vector3(0.0f, GROUND_Y, 0.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);
    currentLane = 1;
    isJumping = false;
    jumpTime = 0.0f;
    jumpHeight = 0.0f;
} 