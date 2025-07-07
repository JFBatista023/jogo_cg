#include "Obstacle.h"
#include "../lighting/Lighting.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

// Estrutura para armazenar dados do modelo OBJ
struct OBJVertex {
    float x, y, z;
};

struct OBJFace {
    int v1, v2, v3;
};

// Variáveis globais para o modelo do alien (carregado uma vez)
static std::vector<OBJVertex> alienVertices;
static std::vector<OBJFace> alienFaces;
static bool alienModelLoaded = false;

// Função para carregar modelo OBJ
bool loadOBJModel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Erro: Não foi possível carregar o modelo " << filename << std::endl;
        return false;
    }
    
    alienVertices.clear();
    alienFaces.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "v") {
            // Vértice
            OBJVertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            alienVertices.push_back(vertex);
        } else if (type == "f") {
            // Face (assumindo triângulos)
            OBJFace face;
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            
            // Extrair apenas o índice do vértice (ignorar textura e normal se existirem)
            face.v1 = std::stoi(v1.substr(0, v1.find('/'))) - 1; // OBJ usa índices baseados em 1
            face.v2 = std::stoi(v2.substr(0, v2.find('/'))) - 1;
            face.v3 = std::stoi(v3.substr(0, v3.find('/'))) - 1;
            alienFaces.push_back(face);
        }
    }
    
    file.close();
    std::cout << "Modelo alien carregado: " << alienVertices.size() << " vértices, " 
              << alienFaces.size() << " faces" << std::endl;
    return true;
}

// Função para renderizar o modelo do alien
void renderAlienModel() {
    if (!alienModelLoaded) {
        alienModelLoaded = loadOBJModel("assets/models/alien_11.obj");
        if (!alienModelLoaded) {
            // Se falhar ao carregar, usar cubo como fallback
            glutSolidCube(1.0f);
            return;
        }
    }
    
    // Renderizar o modelo usando triângulos
    glBegin(GL_TRIANGLES);
    for (const auto& face : alienFaces) {
        if (static_cast<size_t>(face.v1) < alienVertices.size() && 
            static_cast<size_t>(face.v2) < alienVertices.size() && 
            static_cast<size_t>(face.v3) < alienVertices.size()) {
            
            const OBJVertex& v1 = alienVertices[face.v1];
            const OBJVertex& v2 = alienVertices[face.v2];
            const OBJVertex& v3 = alienVertices[face.v3];
            
            // Calcular normal para a face
            float nx = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y);
            float ny = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
            float nz = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
            
            // Normalizar
            float length = sqrt(nx*nx + ny*ny + nz*nz);
            if (length > 0) {
                nx /= length;
                ny /= length;
                nz /= length;
            }
            
            glNormal3f(nx, ny, nz);
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }
    }
    glEnd();
}

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
            // Obstáculo alien - verde alienígena metálico
            Lighting::setObstacleMaterial(0.2f, 0.8f, 0.2f, true); // Verde metálico
            
            // Renderizar modelo do alien (elevado, reduzido pela metade e rotacionado)
            glPushMatrix();
            glTranslatef(0.0f, size.y * 1.1f, 0.0f); // Elevar o alien
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f); // Rotacionar 180 graus no eixo Z
            glScalef(size.x * 0.2f, size.y * 0.2f, size.z * 0.2f);
            renderAlienModel();
            glPopMatrix();
            
            // Efeito de energia alien - material energético verde
            Lighting::setEnergyMaterial(0.3f, 1.0f, 0.3f, 1.5f);
            glPushMatrix();
            glScalef(size.x * 0.9f, size.y * 0.9f, size.z * 0.9f);
            // Aura energética ao redor do alien
            glDisable(GL_LIGHTING);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.0f, 1.0f, 0.0f, 0.3f); // Verde translúcido
            glutSolidCube(1.0f);
            glDisable(GL_BLEND);
            glEnable(GL_LIGHTING);
            glPopMatrix();
            
            // Contorno de alerta alienígena (sem iluminação)
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 1.0f, 0.0f); // Verde brilhante
            glLineWidth(3.0f);
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutWireCube(1.0f);
            glPopMatrix();
            glEnable(GL_LIGHTING);
            break;
            
        case MOVING_VERTICAL:
            // Obstáculo móvel - laranja metálico energético
            Lighting::setObstacleMaterial(1.0f, 0.5f, 0.0f, true); // Laranja metálico
            
            // Renderizar cubo principal
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Efeito de movimento energético
            Lighting::setEnergyMaterial(1.0f, 0.8f, 0.2f, 2.0f);
            glPushMatrix();
            glScalef(size.x * 0.8f, size.y * 0.8f, size.z * 0.8f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Núcleo brilhante
            Lighting::setEnergyMaterial(1.0f, 1.0f, 0.5f, 3.0f);
            glPushMatrix();
            glScalef(size.x * 0.4f, size.y * 0.4f, size.z * 0.4f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Contorno ciano (sem iluminação)
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 1.0f, 1.0f);
            glLineWidth(2.0f);
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutWireCube(1.0f);
            glPopMatrix();
            glEnable(GL_LIGHTING);
            break;
            
        case ROCKET:
            // Foguete espacial - cinza metálico com detalhes realistas
            Lighting::setObstacleMaterial(0.6f, 0.6f, 0.6f, true); // Corpo principal cinza metálico
            
            // Corpo principal do foguete (mais longo)
            glPushMatrix();
            glScalef(size.x * 0.8f, size.y * 0.8f, size.z * 2.5f); // Proporção ajustada para altura maior
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Nose cone (ponta do foguete) - metal polido
            Lighting::setObstacleMaterial(0.8f, 0.8f, 0.8f, true);
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, size.z * 1.25f); // Posicionar na frente
            glScalef(size.x * 0.4f, size.y * 0.4f, size.z * 0.8f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Asas laterais - metal escuro
            Lighting::setObstacleMaterial(0.5f, 0.5f, 0.5f, true);
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
            
            // Motor (parte traseira) - metal escuro
            Lighting::setObstacleMaterial(0.3f, 0.3f, 0.3f, true);
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -size.z * 1.25f);
            glScalef(size.x * 0.6f, size.y * 0.6f, size.z * 0.6f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Chama do motor (efeito de propulsão) - energia pura
            Lighting::setEnergyMaterial(1.0f, 0.3f, 0.0f, 2.5f); // Laranja brilhante
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -size.z * 1.8f);
            glScalef(size.x * 0.3f, size.y * 0.3f, size.z * 0.8f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Chama interna mais brilhante - núcleo energético
            Lighting::setEnergyMaterial(1.0f, 0.8f, 0.0f, 3.0f); // Amarelo muito brilhante
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -size.z * 1.9f);
            glScalef(size.x * 0.15f, size.y * 0.15f, size.z * 0.6f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Contorno de perigo (sem iluminação)
            glDisable(GL_LIGHTING);
            glColor3f(1.0f, 0.0f, 0.0f);
            glLineWidth(3.0f);
            glPushMatrix();
            glScalef(size.x, size.y, size.z * 2.5f);
            glutWireCube(1.0f);
            glPopMatrix();
            glEnable(GL_LIGHTING);
            break;
            
        case HIGH_OBSTACLE:
            // Obstáculo alto - material roxo metálico ameaçador
            Lighting::setObstacleMaterial(0.5f, 0.0f, 0.8f, true); // Roxo metálico escuro
            
            // Corpo principal alto
            glPushMatrix();
            glScalef(size.x, size.y, size.z);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Detalhes estruturais - metal mais claro
            Lighting::setObstacleMaterial(0.7f, 0.2f, 1.0f, true); // Roxo mais claro
            glPushMatrix();
            glScalef(size.x * 0.8f, size.y * 0.8f, size.z * 0.8f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Pontas afiadas no topo - material energético perigoso
            Lighting::setEnergyMaterial(1.0f, 0.0f, 0.5f, 2.0f); // Rosa/vermelho energético
            glPushMatrix();
            glTranslatef(0.0f, size.y * 0.4f, 0.0f);
            glScalef(size.x * 0.3f, size.y * 0.2f, size.z * 0.3f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Efeito de energia (sem iluminação para efeito especial)
            glDisable(GL_LIGHTING);
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
            glEnable(GL_LIGHTING);
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