#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../utils/Vector3.h"
#include <GL/glut.h>

enum ObstacleType {
    STATIC,
    MOVING_VERTICAL
};

class Obstacle {
private:
    Vector3 position;
    Vector3 size;
    ObstacleType type;
    float speed;
    float oscillationAmplitude;
    float oscillationSpeed;
    float time;
    float originalY;
    bool active;
    
public:
    Obstacle();
    Obstacle(Vector3 pos, Vector3 sz, ObstacleType t);
    ~Obstacle();
    
    void update(float deltaTime);
    void render() const;
    void reset(Vector3 pos, Vector3 sz, ObstacleType t);
    
    // Getters
    Vector3 getPosition() const { return position; }
    Vector3 getSize() const { return size; }
    bool isActive() const { return active; }
    
    // Para detecção de colisão
    Vector3 getMin() const;
    Vector3 getMax() const;
    
    void setActive(bool state) { active = state; }
    void moveZ(float deltaZ) { position.z += deltaZ; }
};

#endif 