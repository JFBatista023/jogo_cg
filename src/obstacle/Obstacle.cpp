#include "Obstacle.h"
#include <cmath>

Obstacle::Obstacle() {
    position = Vector3(0.0f, 0.0f, 0.0f);
    size = Vector3(1.0f, 1.0f, 1.0f);
    type = STATIC;
    speed = 0.0f;
    oscillationAmplitude = 2.0f;
    oscillationSpeed = 3.0f;
    time = 0.0f;
    originalY = 0.0f;
    active = false;
}

Obstacle::Obstacle(Vector3 pos, Vector3 sz, ObstacleType t) {
    position = pos;
    size = sz;
    type = t;
    speed = 0.0f;
    oscillationAmplitude = 2.0f;
    oscillationSpeed = 3.0f;
    time = 0.0f;
    originalY = pos.y;
    active = true;
}

Obstacle::~Obstacle() {
}

void Obstacle::update(float deltaTime) {
    if (!active) return;
    
    time += deltaTime;
    
    // Comportamento baseado no tipo
    switch (type) {
        case STATIC:
            // Obstáculo estático não se move
            break;
            
        case MOVING_VERTICAL:
            // Movimento vertical usando função seno
            position.y = originalY + sin(time * oscillationSpeed) * oscillationAmplitude;
            break;
    }
}

void Obstacle::render() const {
    if (!active) return;
    
    glPushMatrix();
    
    // Posicionar o obstáculo
    glTranslatef(position.x, position.y, position.z);
    
    // Renderização baseada no tipo
    switch (type) {
        case STATIC:
            // Obstáculo estático - vermelho brilhante
            glColor3f(1.0f, 0.0f, 0.0f);
            
            // Renderizar cubo principal
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Efeito de perigo
            glColor3f(1.0f, 0.3f, 0.3f);
            glPushMatrix();
            glScalef(size.x * 0.7f, size.y * 0.7f, size.z * 0.7f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Contorno amarelo de alerta
            glColor3f(1.0f, 1.0f, 0.0f);
            glLineWidth(3.0f);
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutWireCube(1.0f);
            glPopMatrix();
            break;
            
        case MOVING_VERTICAL:
            // Obstáculo móvel - laranja brilhante
            glColor3f(1.0f, 0.5f, 0.0f);
            
            // Renderizar cubo principal
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Efeito de movimento
            glColor3f(1.0f, 0.8f, 0.2f);
            glPushMatrix();
            glScalef(size.x * 0.8f, size.y * 0.8f, size.z * 0.8f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Núcleo brilhante
            glColor3f(1.0f, 1.0f, 0.5f);
            glPushMatrix();
            glScalef(size.x * 0.4f, size.y * 0.4f, size.z * 0.4f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Contorno ciano
            glColor3f(0.0f, 1.0f, 1.0f);
            glLineWidth(2.0f);
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutWireCube(1.0f);
            glPopMatrix();
            break;
    }
    
    glPopMatrix();
}

void Obstacle::reset(Vector3 pos, Vector3 sz, ObstacleType t) {
    position = pos;
    size = sz;
    type = t;
    originalY = pos.y;
    time = 0.0f;
    active = true;
}

Vector3 Obstacle::getMin() const {
    return Vector3(position.x - size.x/2, position.y - size.y/2, position.z - size.z/2);
}

Vector3 Obstacle::getMax() const {
    return Vector3(position.x + size.x/2, position.y + size.y/2, position.z + size.z/2);
} 