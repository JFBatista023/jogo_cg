#include "Scene.h"
#include <cstdlib>
#include <ctime>

Scene::Scene() {
    floorSpeed = 20.0f;
    floorLength = 10.0f;
    floorWidth = 10.0f;
    numSections = 10;
    numStars = 200;
}

Scene::~Scene() {
}

void Scene::init() {
    // Inicializar seções do chão
    floorSections.clear();
    for (int i = 0; i < numSections; i++) {
        Vector3 pos(0.0f, 0.0f, -i * floorLength);
        Vector3 size(floorWidth, 0.2f, floorLength);
        floorSections.push_back(FloorSection(pos, size));
    }
    
    // Gerar estrelas
    generateStars();
}

void Scene::update(float deltaTime) {
    updateFloorSections(deltaTime);
}

void Scene::render() {
    renderBackground();
    renderStars();
    renderFloor();
}

void Scene::renderBackground() {
    // Fundo MUITO MAIS CLARO
    glClearColor(0.7f, 0.8f, 1.0f, 1.0f);
    
    // Renderizar gradiente claro
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glBegin(GL_QUADS);
    // Gradiente claro de azul para branco
    glColor3f(0.6f, 0.8f, 1.0f);  // Azul claro no topo
    glVertex2f(0, 1);
    glVertex2f(1, 1);
    glColor3f(0.8f, 0.9f, 1.0f);  // Azul muito claro no meio
    glVertex2f(1, 0.5f);
    glVertex2f(0, 0.5f);
    glColor3f(0.9f, 0.95f, 1.0f); // Quase branco embaixo
    glVertex2f(1, 0);
    glVertex2f(0, 0);
    glEnd();
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void Scene::renderStars() {
    // Estrelas ESCURAS para contraste no fundo claro
    glColor3f(0.0f, 0.0f, 0.5f);  // Azul escuro
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (const auto& star : stars) {
        glVertex3f(star.x, star.y, star.z);
    }
    glEnd();
}

void Scene::renderFloor() {
    for (const auto& section : floorSections) {
        if (!section.active) continue;
        
        glPushMatrix();
        glTranslatef(section.position.x, section.position.y, section.position.z);
        
        // Cor do chão mais vibrante (azul metálico)
        glColor3f(0.3f, 0.5f, 0.7f);
        
        // Renderizar seção do chão
        glPushMatrix();
        glScalef(section.size.x, section.size.y, section.size.z);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Contorno do chão
        glColor3f(0.1f, 0.3f, 0.5f);
        glPushMatrix();
        glScalef(section.size.x, section.size.y, section.size.z);
        glutWireCube(1.0f);
        glPopMatrix();
        
        // Linhas das faixas mais brilhantes
        glColor3f(0.0f, 1.0f, 1.0f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        // Linha da faixa esquerda
        glVertex3f(-1.5f, section.size.y/2 + 0.02f, -section.size.z/2);
        glVertex3f(-1.5f, section.size.y/2 + 0.02f, section.size.z/2);
        // Linha da faixa direita
        glVertex3f(1.5f, section.size.y/2 + 0.02f, -section.size.z/2);
        glVertex3f(1.5f, section.size.y/2 + 0.02f, section.size.z/2);
        glEnd();
        
        // Linhas centrais para melhor definição
        glColor3f(1.0f, 1.0f, 0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        // Linha central
        glVertex3f(0.0f, section.size.y/2 + 0.01f, -section.size.z/2);
        glVertex3f(0.0f, section.size.y/2 + 0.01f, section.size.z/2);
        glEnd();
        
        glPopMatrix();
    }
}

void Scene::setupCamera() {
    gluLookAt(0.0f, 8.0f, 15.0f,  // Posição da câmera
              0.0f, 2.0f, 0.0f,   // Ponto para onde olha
              0.0f, 1.0f, 0.0f);  // Vetor up
}

void Scene::generateStars() {
    stars.clear();
    srand(time(NULL));
    
    for (int i = 0; i < numStars; i++) {
        float x = (rand() % 200 - 100) * 2.0f;
        float y = (rand() % 100 + 20) * 2.0f;
        float z = (rand() % 200 - 100) * 2.0f;
        stars.push_back(Vector3(x, y, z));
    }
}

void Scene::updateFloorSections(float deltaTime) {
    for (auto& section : floorSections) {
        section.position.z += floorSpeed * deltaTime;
        
        // Reposicionar seção quando sai da tela
        if (section.position.z > 20.0f) {
            // Encontrar a seção mais atrás para posicionar logo após ela
            float minZ = section.position.z;
            for (const auto& otherSection : floorSections) {
                if (otherSection.position.z < minZ) {
                    minZ = otherSection.position.z;
                }
            }
            // Posicionar logo após a seção mais atrás
            section.position.z = minZ - floorLength;
        }
    }
} 