#include "Player.h"
#include "../lighting/Lighting.h"
#include <cmath>

// Definição das constantes
const float Player::LANE_WIDTH = 3.0f;
const float Player::JUMP_SPEED = 15.0f;
const float Player::GRAVITY = -30.0f;
const float Player::GROUND_Y = 0.975f;
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
    bool sliding = isSliding;
    if (sliding) {
        scaleX *= 1.5f;  // Mais largo
        scaleY *= 0.5f;  // Mais baixo
        scaleZ *= 1.2f;  // Um pouco mais longo
    }
    glScalef(scaleX, scaleY, scaleZ);

    // Animação baseada no tempo global (corrida)
    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    drawCharacter(t, sliding);

    glPopMatrix();
}

// Desenha o boneco segmentado e animado
void Player::drawCharacter(float t, bool sliding) {
    // Cores
    float skin[3]   = {1.0f, 0.85f, 0.6f};
    float shirt[3]  = {0.15f, 0.45f, 1.0f}; // Azul mais vivo
    float pants[3]  = {0.13f, 0.13f, 0.25f};
    float shoes[3]  = {0.95f, 0.25f, 0.25f}; // Tênis vermelhos
    float accent[3] = {1.0f, 0.8f, 0.2f};    // Cinto amarelo
    float gloves[3] = {0.95f, 0.95f, 0.95f}; // Luvas brancas
    float visor[3]  = {0.4f, 0.85f, 1.0f};   // Visor azul claro

    // Proporções "heróicas"
    float bodyH = 1.0f, bodyW = 0.38f, bodyD = 0.26f;
    float headR = 0.23f;
    float armL = 0.62f, armR = 0.11f;
    float legL = 0.95f, legR = 0.14f;
    float footL = 0.22f, footH = 0.09f;

    // Animação de corrida (ângulos)
    float runSpeed = sliding ? 0.0f : 7.0f;
    float legSwing = sliding ? 0.0f : 45.0f * sinf(t * runSpeed);
    float legSwing2 = sliding ? 0.0f : 45.0f * sinf(t * runSpeed + 3.1415f);
    float armSwing = sliding ? 0.0f : 38.0f * sinf(t * runSpeed + 3.1415f);
    float armSwing2 = sliding ? 0.0f : 38.0f * sinf(t * runSpeed);
    float bodyLean = sliding ? 0.0f : 12.0f * sinf(t * runSpeed * 0.5f);

    // Tronco
    glPushMatrix();
    glRotatef(bodyLean, 1, 0, 0);
    glColor3fv(shirt);
    glScalef(bodyW, bodyH, bodyD);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cinto
    glPushMatrix();
    glTranslatef(0.0f, -bodyH*0.23f, 0.0f);
    glColor3fv(accent);
    glScalef(bodyW*1.05f, bodyH*0.13f, bodyD*1.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cabeça
    glPushMatrix();
    glTranslatef(0.0f, bodyH/2 + headR*0.93f, 0.0f);
    glColor3fv(skin);
    glutSolidSphere(headR, 18, 14);
    // Visor futurista
    glPushMatrix();
    glTranslatef(0.0f, 0.03f, headR*0.7f);
    glColor3fv(visor);
    glScalef(0.7f, 0.38f, 0.18f);
    glutSolidSphere(0.5f, 12, 8);
    glPopMatrix();
    glPopMatrix();

    // Braço esquerdo
    glPushMatrix();
    glTranslatef(-(bodyW/2 + armR*0.7f), bodyH/2 - armR*0.1f, 0.0f);
    glRotatef(armSwing, 1, 0, 0);
    // Ombro
    glColor3fv(shirt);
    glutSolidSphere(armR*1.1f, 10, 8);
    // Braço
    glTranslatef(0.0f, -armL/2, 0.0f);
    glPushMatrix();
    glColor3fv(shirt);
    glScalef(armR, armL, armR);
    glutSolidCube(1.0f);
    glPopMatrix();
    // Luva
    glTranslatef(0.0f, -armL/2 - armR*0.13f, 0.0f);
    glColor3fv(gloves);
    glutSolidSphere(armR*0.8f, 10, 8);
    glPopMatrix();

    // Braço direito
    glPushMatrix();
    glTranslatef((bodyW/2 + armR*0.7f), bodyH/2 - armR*0.1f, 0.0f);
    glRotatef(armSwing2, 1, 0, 0);
    // Ombro
    glColor3fv(shirt);
    glutSolidSphere(armR*1.1f, 10, 8);
    // Braço
    glTranslatef(0.0f, -armL/2, 0.0f);
    glPushMatrix();
    glColor3fv(shirt);
    glScalef(armR, armL, armR);
    glutSolidCube(1.0f);
    glPopMatrix();
    // Luva
    glTranslatef(0.0f, -armL/2 - armR*0.13f, 0.0f);
    glColor3fv(gloves);
    glutSolidSphere(armR*0.8f, 10, 8);
    glPopMatrix();

    // Perna esquerda
    glPushMatrix();
    glTranslatef(-bodyW*0.22f, -bodyH/2, 0.0f);
    glRotatef(legSwing, 1, 0, 0);
    // Quadril
    glColor3fv(pants);
    glutSolidSphere(legR*1.1f, 10, 8);
    // Perna
    glTranslatef(0.0f, -legL/2, 0.0f);
    glPushMatrix();
    glColor3fv(pants);
    glScalef(legR, legL, legR);
    glutSolidCube(1.0f);
    glPopMatrix();
    // Tênis
    glTranslatef(0.0f, -legL/2 - footH*0.5f, footL*0.3f);
    glColor3fv(shoes);
    glPushMatrix();
    glScalef(legR*1.2f, footH, footL);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    // Perna direita
    glPushMatrix();
    glTranslatef(bodyW*0.22f, -bodyH/2, 0.0f);
    glRotatef(legSwing2, 1, 0, 0);
    // Quadril
    glColor3fv(pants);
    glutSolidSphere(legR*1.1f, 10, 8);
    // Perna
    glTranslatef(0.0f, -legL/2, 0.0f);
    glPushMatrix();
    glColor3fv(pants);
    glScalef(legR, legL, legR);
    glutSolidCube(1.0f);
    glPopMatrix();
    // Tênis
    glTranslatef(0.0f, -legL/2 - footH*0.5f, footL*0.3f);
    glColor3fv(shoes);
    glPushMatrix();
    glScalef(legR*1.2f, footH, footL);
    glutSolidCube(1.0f);
    glPopMatrix();
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