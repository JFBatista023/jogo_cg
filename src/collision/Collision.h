#ifndef COLLISION_H
#define COLLISION_H

#include "../utils/Vector3.h"
#include "../player/Player.h"
#include "../obstacle/Obstacle.h"
#include <vector>

class Collision {
public:
    // Detecção de colisão AABB (Axis-Aligned Bounding Box)
    static bool checkAABB(const Vector3& min1, const Vector3& max1,
                         const Vector3& min2, const Vector3& max2);
    
    // Sobrecarga para objetos específicos
    static bool checkCollision(const Player& player, const Obstacle& obstacle);
    
    // Verificação de colisão com múltiplos obstáculos
    static bool checkCollisionWithObstacles(const Player& player, 
                                           const std::vector<Obstacle>& obstacles);
    
    // Utilitários para debug
    static void renderBoundingBox(const Vector3& min, const Vector3& max);
    static void renderPlayerBoundingBox(const Player& player);
    static void renderObstacleBoundingBox(const Obstacle& obstacle);
};

#endif 