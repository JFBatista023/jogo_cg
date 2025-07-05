#include "Collision.h"
#include <vector>

bool Collision::checkAABB(const Vector3& min1, const Vector3& max1,
                         const Vector3& min2, const Vector3& max2) {
    return (min1.x <= max2.x && max1.x >= min2.x) &&
           (min1.y <= max2.y && max1.y >= min2.y) &&
           (min1.z <= max2.z && max1.z >= min2.z);
}

bool Collision::checkCollision(const Player& player, const Obstacle& obstacle) {
    if (!obstacle.isActive()) return false;
    
    Vector3 playerMin = player.getMin();
    Vector3 playerMax = player.getMax();
    Vector3 obstacleMin = obstacle.getMin();
    Vector3 obstacleMax = obstacle.getMax();
    
    // Verificar colisão básica AABB
    bool basicCollision = checkAABB(playerMin, playerMax, obstacleMin, obstacleMax);
    
    // Se não há colisão básica, não há colisão
    if (!basicCollision) return false;
    
    // Lógica específica para cada tipo de obstáculo
    switch (obstacle.getType()) {
        case STATIC:
            // Obstáculo vermelho: no chão, pode pular ou desviar
            // Se o jogador está deslizando, MORRE (não pode deslizar sobre obstáculo no chão)
            if (player.getIsSliding()) {
                return true; // Colisão = morte
            }
            // Se não está deslizando, colisão normal
            return basicCollision;
            
        case MOVING_VERTICAL:
            // Obstáculo móvel: comportamento similar ao estático
            if (player.getIsSliding()) {
                return true; // Colisão = morte
            }
            return basicCollision;
            
        case ROCKET:
            // Foguete: muito alto, só pode desviar
            if (player.getIsSliding()) {
                return true; // Colisão = morte
            }
            return basicCollision;
            
        case HIGH_OBSTACLE:
            // Obstáculo roxo: elevado, só pode deslizar embaixo ou desviar
            if (player.getIsSliding()) {
                // Se está deslizando E está embaixo do obstáculo, não há colisão
                if (playerMax.y < obstacleMin.y) {
                    return false; // Passa embaixo
                }
            }
            // Se não está deslizando ou está tocando o obstáculo, há colisão
            return true;
            
        default:
            return basicCollision;
    }
}

bool Collision::checkCollisionWithObstacles(const Player& player, 
                                           const std::vector<Obstacle>& obstacles) {
    for (const auto& obstacle : obstacles) {
        if (checkCollision(player, obstacle)) {
            return true;
        }
    }
    return false;
}

void Collision::renderBoundingBox(const Vector3& min, const Vector3& max) {
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho para debug
    glLineWidth(1.0f);
    
    glBegin(GL_LINES);
    // Arestas inferiores
    glVertex3f(min.x, min.y, min.z);
    glVertex3f(max.x, min.y, min.z);
    
    glVertex3f(max.x, min.y, min.z);
    glVertex3f(max.x, min.y, max.z);
    
    glVertex3f(max.x, min.y, max.z);
    glVertex3f(min.x, min.y, max.z);
    
    glVertex3f(min.x, min.y, max.z);
    glVertex3f(min.x, min.y, min.z);
    
    // Arestas superiores
    glVertex3f(min.x, max.y, min.z);
    glVertex3f(max.x, max.y, min.z);
    
    glVertex3f(max.x, max.y, min.z);
    glVertex3f(max.x, max.y, max.z);
    
    glVertex3f(max.x, max.y, max.z);
    glVertex3f(min.x, max.y, max.z);
    
    glVertex3f(min.x, max.y, max.z);
    glVertex3f(min.x, max.y, min.z);
    
    // Arestas verticais
    glVertex3f(min.x, min.y, min.z);
    glVertex3f(min.x, max.y, min.z);
    
    glVertex3f(max.x, min.y, min.z);
    glVertex3f(max.x, max.y, min.z);
    
    glVertex3f(max.x, min.y, max.z);
    glVertex3f(max.x, max.y, max.z);
    
    glVertex3f(min.x, min.y, max.z);
    glVertex3f(min.x, max.y, max.z);
    glEnd();
}

void Collision::renderPlayerBoundingBox(const Player& player) {
    renderBoundingBox(player.getMin(), player.getMax());
}

void Collision::renderObstacleBoundingBox(const Obstacle& obstacle) {
    if (obstacle.isActive()) {
        renderBoundingBox(obstacle.getMin(), obstacle.getMax());
    }
} 