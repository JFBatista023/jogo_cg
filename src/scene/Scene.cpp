#include "Scene.h"
#include "../texture/Texture.h"
#include <GL/glut.h>
#include <cmath>

Scene::Scene() {
    floorOffset = 0.0f;
    floorSpeed = 20.0f;
}

Scene::~Scene() {
}

void Scene::init() {
    // Inicializar sistema de texturas se ainda não foi
    if (!Texture::exists("galaxy")) {
        Texture::init();
    }
}

void Scene::update(float deltaTime) {
    // Atualizar offset do chão para criar movimento
    floorOffset += floorSpeed * deltaTime;
    
    // Reset quando necessário (evitar overflow)
    if (floorOffset > 100.0f) {
        floorOffset -= 100.0f;
    }
}

void Scene::render() {
    renderSkybox();
    
    renderFloor();
    
    // renderLanes(); // Comentado para remover traços e pontilhados
}

void Scene::renderFloor() {
    glPushMatrix();
    
    // Habilitar texturas
    glEnable(GL_TEXTURE_2D);
    
    // Aplicar textura de galáxia
    Texture::bind("galaxy");
    
    // Configurar cor base para não alterar a textura
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Renderizar múltiplos segmentos do piso para criar continuidade
    float segmentLength = 50.0f;
    int numSegments = 4;
    float textureRepeat = 1.0f;
    
    for (int i = 0; i < numSegments; i++) {
        float z1 = floorOffset - (i * segmentLength);
        float z2 = z1 - segmentLength;
        
        // Calcular offset da textura baseado no movimento
        float texOffsetY = (floorOffset / segmentLength) + i;
        
        glBegin(GL_QUADS);
            // Coordenadas de textura com movimento
            glTexCoord2f(0.0f, texOffsetY);
            glVertex3f(-6.0f, 0.0f, z1);
            
            glTexCoord2f(textureRepeat, texOffsetY);
            glVertex3f(6.0f, 0.0f, z1);
            
            glTexCoord2f(textureRepeat, texOffsetY + textureRepeat);
            glVertex3f(6.0f, 0.0f, z2);
            
            glTexCoord2f(0.0f, texOffsetY + textureRepeat);
            glVertex3f(-6.0f, 0.0f, z2);
        glEnd();
    }
    
    // Desabilitar texturas
    Texture::unbind();
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}

void Scene::renderSkybox() {
    glPushMatrix();
    
    // Habilitar texturas para o skybox
    glEnable(GL_TEXTURE_2D);
    
    // Usar textura de campo de estrelas para o skybox
    Texture::bind("starfield");
    
    // Desabilitar teste de profundidade para o skybox
    glDisable(GL_DEPTH_TEST);
    
    // Configurar cor
    glColor4f(0.8f, 0.8f, 1.0f, 1.0f); // Azul claro
    
    // Renderizar skybox como um grande cubo ao redor do jogador
    float skySize = 500.0f;
    
    // Parede traseira
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-skySize, -skySize, -skySize);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(skySize, -skySize, -skySize);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(skySize, skySize, -skySize);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-skySize, skySize, -skySize);
    glEnd();
    
    // Parede esquerda
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-skySize, -skySize, skySize);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-skySize, -skySize, -skySize);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-skySize, skySize, -skySize);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-skySize, skySize, skySize);
    glEnd();
    
    // Parede direita
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(skySize, -skySize, -skySize);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(skySize, -skySize, skySize);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(skySize, skySize, skySize);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(skySize, skySize, -skySize);
    glEnd();
    
    // Teto com textura de nebulosa
    Texture::bind("nebula");
    glColor4f(0.6f, 0.4f, 0.8f, 1.0f); // Roxo
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-skySize, skySize, -skySize);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(skySize, skySize, -skySize);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(skySize, skySize, skySize);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-skySize, skySize, skySize);
    glEnd();
    
    // Reabilitar teste de profundidade
    glEnable(GL_DEPTH_TEST);
    
    // Desabilitar texturas
    Texture::unbind();
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}

void Scene::renderLanes() {
    // FUNÇÃO COMENTADA - Remove traços e pontilhados da pista
    // Deixa apenas a textura de galáxia limpa
    /*
    // Renderizar linhas das faixas sem textura
    glDisable(GL_TEXTURE_2D);
    
    // Linhas das faixas - amarelas brilhantes
    glColor4f(1.0f, 1.0f, 0.0f, 0.8f);
    glLineWidth(3.0f);
    
    // Renderizar linhas tracejadas
    float lineLength = 2.0f;
    float gapLength = 1.0f;
    float totalLength = lineLength + gapLength;
    
    for (int lane = 0; lane < 2; lane++) {
        float x = (lane - 0.5f) * 6.0f; // Posições -3 e 3
        
        glBegin(GL_LINES);
        for (float z = floorOffset; z > floorOffset - 100.0f; z -= totalLength) {
            glVertex3f(x, 0.01f, z);
            glVertex3f(x, 0.01f, z - lineLength);
        }
        glEnd();
    }
    
    // Bordas da pista - brancas
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(4.0f);
    
    glBegin(GL_LINES);
    // Borda esquerda
    glVertex3f(-6.0f, 0.01f, floorOffset);
    glVertex3f(-6.0f, 0.01f, floorOffset - 100.0f);
    
    // Borda direita
    glVertex3f(6.0f, 0.01f, floorOffset);
    glVertex3f(6.0f, 0.01f, floorOffset - 100.0f);
    glEnd();
    */
}

void Scene::setupCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Câmera em terceira pessoa atrás do jogador
    gluLookAt(
        0.0f, 8.0f, 15.0f,    // Posição da câmera
        0.0f, 2.0f, 0.0f,     // Ponto para onde olha
        0.0f, 1.0f, 0.0f      // Vetor up
    );
}

void Scene::setFloorSpeed(float speed) {
    floorSpeed = speed;
}

float Scene::getFloorSpeed() const {
    return floorSpeed;
}