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
            
        case ROCKET:
            // Foguete se move ligeiramente para cima e para baixo
            position.y = originalY + sin(time * oscillationSpeed * 0.5f) * (oscillationAmplitude * 0.3f);
            break;
            
        case HIGH_OBSTACLE:
            // Obstáculo alto tem movimento sutil de flutuação
            position.y = originalY + sin(time * oscillationSpeed * 0.3f) * (oscillationAmplitude * 0.2f);
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
            
        case ROCKET:
            // Foguete espacial - cinza metálico com detalhes
            glColor3f(0.6f, 0.6f, 0.6f); // Corpo principal cinza
            
            // Corpo principal do foguete (mais longo)
            glPushMatrix();
            glScalef(size.x * 0.8f, size.y * 0.8f, size.z * 2.5f); // Proporção ajustada para altura maior
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Nose cone (ponta do foguete)
            glColor3f(0.8f, 0.8f, 0.8f);
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, size.z * 1.25f); // Posicionar na frente
            glScalef(size.x * 0.4f, size.y * 0.4f, size.z * 0.8f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Asas laterais
            glColor3f(0.5f, 0.5f, 0.5f);
            // Asa esquerda
            glPushMatrix();
            glTranslatef(-size.x * 0.6f, 0.0f, 0.0f);
            glScalef(size.x * 0.3f, size.y * 0.6f, size.z * 0.8f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Asa direita
            glPushMatrix();
            glTranslatef(size.x * 0.6f, 0.0f, 0.0f);
            glScalef(size.x * 0.3f, size.y * 0.6f, size.z * 0.8f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Motor (parte traseira)
            glColor3f(0.3f, 0.3f, 0.3f);
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -size.z * 1.25f);
            glScalef(size.x * 0.6f, size.y * 0.6f, size.z * 0.6f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Chama do motor (efeito de propulsão)
            glColor3f(1.0f, 0.3f, 0.0f); // Laranja
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -size.z * 1.8f);
            glScalef(size.x * 0.3f, size.y * 0.3f, size.z * 0.8f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Chama interna mais brilhante
            glColor3f(1.0f, 0.8f, 0.0f); // Amarelo
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -size.z * 1.9f);
            glScalef(size.x * 0.15f, size.y * 0.15f, size.z * 0.6f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Contorno vermelho de perigo
            glColor3f(1.0f, 0.0f, 0.0f);
            glLineWidth(3.0f);
            glPushMatrix();
            glScalef(size.x, size.y, size.z * 2.5f);
            glutWireCube(1.0f);
            glPopMatrix();
            break;
            
        case HIGH_OBSTACLE:
            // Obstáculo alto - roxo escuro e ameaçador
            glColor3f(0.5f, 0.0f, 0.8f); // Roxo escuro
            
            // Corpo principal alto
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Detalhes estruturais
            glColor3f(0.7f, 0.2f, 1.0f); // Roxo mais claro
            glPushMatrix();
            glScalef(size.x * 0.8f, size.y * 0.8f, size.z * 0.8f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Pontas afiadas no topo
            glColor3f(1.0f, 0.0f, 0.5f); // Rosa/vermelho
            glPushMatrix();
            glTranslatef(0.0f, size.y * 0.4f, 0.0f);
            glScalef(size.x * 0.3f, size.y * 0.2f, size.z * 0.3f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Efeito de energia
            glColor3f(0.0f, 1.0f, 1.0f); // Ciano
            glLineWidth(2.0f);
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutWireCube(1.0f);
            glPopMatrix();
            
            // Linhas de energia
            glBegin(GL_LINES);
            glVertex3f(-size.x * 0.4f, size.y * 0.3f, -size.z * 0.4f);
            glVertex3f(size.x * 0.4f, size.y * 0.3f, size.z * 0.4f);
            glVertex3f(size.x * 0.4f, size.y * 0.3f, -size.z * 0.4f);
            glVertex3f(-size.x * 0.4f, size.y * 0.3f, size.z * 0.4f);
            glEnd();
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