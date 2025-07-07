#include "Player.h"
#include "../lighting/Lighting.h"
#include <cmath>

// Definição das constantes
const float Player::LANE_WIDTH = 3.0f;
const float Player::JUMP_SPEED = 15.0f;
const float Player::GRAVITY = -30.0f;
const float Player::GROUND_Y = 0.5f;
const float Player::SLIDE_DURATION = 0.8f;
const float Player::SLIDE_HEIGHT = 0.3f;
const float Player::LANE_TRANSITION_SPEED = 8.0f;

Player::Player() {
    position = Vector3(0.0f, GROUND_Y, 0.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);
    currentLane = 1; // Começa no centro
    targetLane = 1;  // Inicialmente igual ao current
    laneTransitionTime = 0.0f;
    laneTransitionSpeed = LANE_TRANSITION_SPEED;
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
    // Atualizar transição suave entre faixas com interpolação suave
    if (currentLane != targetLane) {
        laneTransitionTime += deltaTime;
        
        float currentX = (currentLane - 1) * LANE_WIDTH;
        float targetX = (targetLane - 1) * LANE_WIDTH;
        
        // Usar interpolação suave (smoothstep) para transição mais natural
        float transitionDuration = 1.0f / laneTransitionSpeed;
        float t = laneTransitionTime / transitionDuration;
        
        if (t >= 1.0f) {
            // Transição completa
            position.x = targetX;
            currentLane = targetLane;
            laneTransitionTime = 0.0f;
        } else {
            // Aplicar curva de suavização (smoothstep)
            float smoothT = t * t * (3.0f - 2.0f * t);
            position.x = currentX + (targetX - currentX) * smoothT;
        }
    } else {
        // Garantir que está exatamente na posição da faixa quando não está em transição
        float targetX = (currentLane - 1) * LANE_WIDTH;
        position.x = targetX;
        laneTransitionTime = 0.0f;
    }
    
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
    
    // Material do jogador (azul ciano brilhante) - usando função utilitária
    Lighting::setPlayerMaterial(0.0f, 0.8f, 1.0f, 0.8f);
    
    // Renderizar cubo principal
    glPushMatrix();
    glScalef(scaleX, scaleY, scaleZ);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Efeito de brilho interno - material energético mais brilhante
    Lighting::setEnergyMaterial(0.5f, 1.0f, 1.0f, 1.5f);
    glPushMatrix();
    glScalef(scaleX * 0.8f, scaleY * 0.8f, scaleZ * 0.8f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Núcleo brilhante - material energético muito brilhante
    Lighting::setEnergyMaterial(1.0f, 1.0f, 1.0f, 3.0f);
    glPushMatrix();
    glScalef(scaleX * 0.3f, scaleY * 0.3f, scaleZ * 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Desabilitar iluminação para efeitos
    glDisable(GL_LIGHTING);
    
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
    
    // Reabilitar iluminação
    glEnable(GL_LIGHTING);
    
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
    if (targetLane > 0) {
        targetLane--;
    }
}

void Player::moveRight() {
    if (targetLane < 2) {
        targetLane++;
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
    targetLane = 1;
    laneTransitionTime = 0.0f;
    isJumping = false;
    isSliding = false;
    jumpTime = 0.0f;
    slideTime = 0.0f;
    jumpHeight = 0.0f;
} 