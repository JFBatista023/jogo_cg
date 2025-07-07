#include "Player.h"
#include "../lighting/Lighting.h"
#include <cmath>
#include <iostream>

// Definição das constantes
const float Player::LANE_WIDTH = 3.0f;
const float Player::JUMP_SPEED = 15.0f;
const float Player::GRAVITY = -30.0f;
const float Player::GROUND_Y = 1.2f; // ELEVADO (era 0.5f)
const float Player::SLIDE_DURATION = 0.8f;
const float Player::SLIDE_HEIGHT = 0.3f;
const float Player::LANE_TRANSITION_SPEED = 8.0f;

Player::Player() {
    try {
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
        
        // Inicializar variáveis de animação
        animationTime = 0.0f;
        runCycleSpeed = 4.0f; // Velocidade do ciclo de corrida
        armSwingAngle = 0.0f;
        legSwingAngle = 0.0f;
        bodyBobHeight = 0.0f;
        
        std::cout << "Player inicializado com sucesso!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erro ao inicializar Player: " << e.what() << std::endl;
    }
}

Player::~Player() {
    std::cout << "Player destruído." << std::endl;
}

void Player::update(float deltaTime) {
    try {
        // Verificar se deltaTime é válido
        if (deltaTime < 0.0f || deltaTime > 1.0f) {
            return; // Ignorar frames com deltaTime inválido
        }
        
        // Atualizar tempo de animação
        animationTime += deltaTime;
        
        // Calcular animações de corrida
        float runPhase = sin(animationTime * runCycleSpeed);
        armSwingAngle = runPhase * 30.0f; // Balanço dos braços (±30 graus)
        legSwingAngle = runPhase * 25.0f; // Balanço das pernas (±25 graus)
        bodyBobHeight = abs(sin(animationTime * runCycleSpeed * 2.0f)) * 0.1f; // Balanceio vertical do corpo
        
        // Atualizar transição entre faixas
        if (targetLane != currentLane) {
            laneTransitionTime += deltaTime * laneTransitionSpeed;
            
            if (laneTransitionTime >= 1.0f) {
                // Transição completa
                currentLane = targetLane;
                laneTransitionTime = 0.0f;
                
                // Posição exata da faixa
                float targetX = (currentLane - 1) * LANE_WIDTH;
                position.x = targetX;
            } else {
                // Interpolação suave
                float currentX = (currentLane - 1) * LANE_WIDTH;
                float targetX = (targetLane - 1) * LANE_WIDTH;
                float smoothT = 1.0f - pow(1.0f - laneTransitionTime, 3.0f); // Easing cúbico
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
    } catch (const std::exception& e) {
        std::cerr << "Erro no Player::update: " << e.what() << std::endl;
    }
}

void Player::render() {
    try {
        glPushMatrix();
        
        // Posicionar o astronauta (ELEVADO e MAIS PARA TRÁS)
        glTranslatef(position.x, position.y + bodyBobHeight + 0.3f, position.z + 3.0f); // +3.0f para mover para trás
        
        // Inclinação leve durante o deslize
        if (isSliding) {
            glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
        }
        
        // Renderizar corpo principal (tronco)
        renderAstronautTorso();
        
        // Renderizar cabeça
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f); // Cabeça um pouco mais baixa (era 1.2f)
        renderAstronautHead();
        glPopMatrix();
        
        // Renderizar jetpack
        glPushMatrix();
        glTranslatef(0.0f, 0.4f, -0.5f); // Jetpack um pouco mais baixo
        renderAstronautJetpack();
        glPopMatrix();
        
        // Renderizar braços - POSIÇÕES AJUSTADAS
        glPushMatrix();
        glTranslatef(-0.35f, 0.5f, 0.0f); // Braço esquerdo mais próximo
        renderAstronautArm(true, armSwingAngle);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.35f, 0.5f, 0.0f); // Braço direito mais próximo
        renderAstronautArm(false, -armSwingAngle); // Movimento inverso
        glPopMatrix();
        
        // Renderizar pernas - POSIÇÕES AJUSTADAS PARA PERNAS MENORES
        glPushMatrix();
        glTranslatef(-0.12f, -0.45f, 0.0f); // Perna esquerda
        renderAstronautLeg(true, legSwingAngle);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.12f, -0.45f, 0.0f); // Perna direita
        renderAstronautLeg(false, -legSwingAngle); // Movimento inverso
        glPopMatrix();
        
        glPopMatrix();
    } catch (const std::exception& e) {
        std::cerr << "Erro no Player::render: " << e.what() << std::endl;
        // Fallback: renderizar um cubo simples
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z + 3.0f); // Também mover o fallback para trás
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
}

void Player::renderAstronautHead() {
    try {
        // Verificar se as funções de material existem
        if (!glIsEnabled(GL_LIGHTING)) {
            glEnable(GL_LIGHTING);
        }
        
        // Capacete - esfera transparente azulada
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Material do capacete (azul transparente) - fallback para cor básica
        glColor4f(0.3f, 0.6f, 1.0f, 0.3f);
        
        glPushMatrix();
        glScalef(0.5f, 0.5f, 0.5f);
        glutSolidSphere(1.0, 16, 16);
        glPopMatrix();
        
        glDisable(GL_BLEND);
        
        // Cabeça do astronauta (dentro do capacete)
        glColor3f(1.0f, 0.8f, 0.7f); // Cor de pele
        glPushMatrix();
        glScalef(0.35f, 0.35f, 0.35f);
        glutSolidSphere(1.0, 12, 12);
        glPopMatrix();
        
        // Reflexo do capacete
        glDisable(GL_LIGHTING);
        glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
        glEnable(GL_BLEND);
        glPushMatrix();
        glTranslatef(-0.1f, 0.1f, 0.3f);
        glScalef(0.15f, 0.2f, 0.1f);
        glutSolidSphere(1.0, 8, 8);
        glPopMatrix();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    } catch (const std::exception& e) {
        std::cerr << "Erro no renderAstronautHead: " << e.what() << std::endl;
        // Fallback simples
        glColor3f(0.8f, 0.8f, 0.8f);
        glutSolidSphere(0.3f, 8, 8);
    }
}

void Player::renderAstronautTorso() {
    try {
        // Tronco principal - traje espacial branco
        glColor3f(0.9f, 0.9f, 0.9f);
        
        glPushMatrix();
        glScalef(0.6f, 0.8f, 0.4f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Painel de controle no peito (azul)
        glColor3f(0.0f, 0.5f, 1.0f);
        glPushMatrix();
        glTranslatef(0.0f, 0.2f, 0.25f);
        glScalef(0.3f, 0.2f, 0.05f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Luzes do painel (verde)
        glDisable(GL_LIGHTING);
        glColor3f(0.0f, 1.0f, 0.0f);
        
        glPushMatrix();
        glTranslatef(-0.08f, 0.25f, 0.28f);
        glutSolidSphere(0.02f, 6, 6);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.08f, 0.25f, 0.28f);
        glutSolidSphere(0.02f, 6, 6);
        glPopMatrix();
        
        glEnable(GL_LIGHTING);
    } catch (const std::exception& e) {
        std::cerr << "Erro no renderAstronautTorso: " << e.what() << std::endl;
        // Fallback simples
        glColor3f(0.9f, 0.9f, 0.9f);
        glutSolidCube(0.8f);
    }
}

void Player::renderAstronautArm(bool isLeft, float swingAngle) {
    try {
        // Evitar warning de parâmetro não usado
        (void)isLeft;
        
        glPushMatrix();
        
        // Aplicar rotação de balanço no ombro
        glRotatef(swingAngle, 1.0f, 0.0f, 0.0f);
        
        // Braço superior - branco (REDUZIDO)
        glColor3f(0.9f, 0.9f, 0.9f);
        glPushMatrix();
        glTranslatef(0.0f, -0.15f, 0.0f); // Mais próximo do ombro
        glScalef(0.12f, 0.3f, 0.12f); // Menor (era 0.15f, 0.5f, 0.15f)
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Cotovelo - cinza (AJUSTADO)
        glColor3f(0.6f, 0.6f, 0.6f);
        glPushMatrix();
        glTranslatef(0.0f, -0.3f, 0.0f); // Posição ajustada
        glutSolidSphere(0.08f, 8, 8); // Menor (era 0.1f)
        glPopMatrix();
        
        // Antebraço - branco (REDUZIDO)
        glColor3f(0.9f, 0.9f, 0.9f);
        glPushMatrix();
        glTranslatef(0.0f, -0.45f, 0.0f); // Posição ajustada
        glScalef(0.1f, 0.3f, 0.1f); // Menor (era 0.12f, 0.5f, 0.12f)
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Luva - azul (AJUSTADA)
        glColor3f(0.0f, 0.3f, 0.8f);
        glPushMatrix();
        glTranslatef(0.0f, -0.65f, 0.0f); // Posição ajustada
        glScalef(0.15f, 0.15f, 0.15f); // Menor (era 0.18f, 0.2f, 0.18f)
        glutSolidCube(1.0f);
        glPopMatrix();
        
        glPopMatrix();
    } catch (const std::exception& e) {
        std::cerr << "Erro no renderAstronautArm: " << e.what() << std::endl;
    }
}

void Player::renderAstronautLeg(bool isLeft, float swingAngle) {
    try {
        // Evitar warning de parâmetro não usado
        (void)isLeft;
        
        glPushMatrix();
        
        // Aplicar rotação de balanço no quadril
        glRotatef(swingAngle, 1.0f, 0.0f, 0.0f);
        
        // Coxa - branco (REDUZIDA AINDA MAIS)
        glColor3f(0.9f, 0.9f, 0.9f);
        glPushMatrix();
        glTranslatef(0.0f, -0.2f, 0.0f); // Mais próximo do quadril
        glScalef(0.15f, 0.4f, 0.15f); // Menor (era 0.18f, 0.5f, 0.18f)
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Joelho - cinza (AJUSTADO)
        glColor3f(0.6f, 0.6f, 0.6f);
        glPushMatrix();
        glTranslatef(0.0f, -0.4f, 0.0f); // Posição ajustada
        glutSolidSphere(0.08f, 8, 8); // Menor (era 0.1f)
        glPopMatrix();
        
        // Panturrilha - branco (REDUZIDA AINDA MAIS)
        glColor3f(0.9f, 0.9f, 0.9f);
        glPushMatrix();
        glTranslatef(0.0f, -0.6f, 0.0f); // Posição ajustada
        glScalef(0.12f, 0.4f, 0.12f); // Menor (era 0.15f, 0.5f, 0.15f)
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Bota espacial - azul escuro (REDUZIDA)
        glColor3f(0.0f, 0.2f, 0.6f);
        glPushMatrix();
        glTranslatef(0.0f, -0.85f, 0.05f); // Posição ajustada
        glScalef(0.18f, 0.1f, 0.25f); // Menor (era 0.2f, 0.12f, 0.3f)
        glutSolidCube(1.0f);
        glPopMatrix();
        
        glPopMatrix();
    } catch (const std::exception& e) {
        std::cerr << "Erro no renderAstronautLeg: " << e.what() << std::endl;
    }
}

void Player::renderAstronautJetpack() {
    try {
        // Corpo principal do jetpack - cinza metálico
        glColor3f(0.4f, 0.4f, 0.5f);
        glPushMatrix();
        glScalef(0.4f, 0.6f, 0.2f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Tubos de propulsão (esquerdo e direito) - usando cubos
        glColor3f(0.3f, 0.3f, 0.4f);
        
        glPushMatrix();
        glTranslatef(-0.15f, -0.3f, 0.0f);
        glScalef(0.08f, 0.3f, 0.08f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.15f, -0.3f, 0.0f);
        glScalef(0.08f, 0.3f, 0.08f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Efeitos de propulsão (durante o pulo)
        if (isJumping) {
            glDisable(GL_LIGHTING);
            
            // Chama azul
            glColor4f(0.0f, 0.5f, 1.0f, 0.7f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glPushMatrix();
            glTranslatef(-0.15f, -0.7f, 0.0f);
            glScalef(0.06f, 0.3f, 0.06f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0.15f, -0.7f, 0.0f);
            glScalef(0.06f, 0.3f, 0.06f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            glDisable(GL_BLEND);
            glEnable(GL_LIGHTING);
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro no renderAstronautJetpack: " << e.what() << std::endl;
    }
}

void Player::jump() {
    if (!isJumping && !isSliding) {
        isJumping = true;
        velocity.y = JUMP_SPEED;
        jumpTime = 0.0f;
    }
}

void Player::slide() {
    if (!isJumping && !isSliding) {
        isSliding = true;
        slideTime = 0.0f;
    }
}

void Player::moveLeft() {
    if (targetLane > 0 && laneTransitionTime == 0.0f) {
        targetLane--;
    }
}

void Player::moveRight() {
    if (targetLane < 2 && laneTransitionTime == 0.0f) {
        targetLane++;
    }
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
    animationTime = 0.0f;
}

// Para colisão - apenas os métodos que NÃO estão definidos inline no .h
Vector3 Player::getMin() const {
    Vector3 size = getSize();
    // Ajustar a posição Y para considerar a elevação do personagem
    // E ajustar a posição Z para considerar o deslocamento para trás
    Vector3 adjustedPos = position;
    adjustedPos.y += 0.3f; // Mesma elevação usada no render
    adjustedPos.z += 3.0f; // Mesmo deslocamento Z usado no render (para trás)
    return Vector3(adjustedPos.x - size.x/2, adjustedPos.y - size.y/2, adjustedPos.z - size.z/2);
}

Vector3 Player::getMax() const {
    Vector3 size = getSize();
    // Ajustar a posição Y para considerar a elevação do personagem
    // E ajustar a posição Z para considerar o deslocamento para trás
    Vector3 adjustedPos = position;
    adjustedPos.y += 0.3f; // Mesma elevação usada no render
    adjustedPos.z += 3.0f; // Mesmo deslocamento Z usado no render (para trás)
    return Vector3(adjustedPos.x + size.x/2, adjustedPos.y + size.y/2, adjustedPos.z + size.z/2);
}