#include "Obstacle.h"
#include "../lighting/Lighting.h"
#include "../texture/Texture.h"
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
    float nx, ny, nz; // Normal pré-calculada
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
            
            // Calcular normal da face uma vez durante o carregamento
            if (static_cast<size_t>(face.v1) < alienVertices.size() && 
                static_cast<size_t>(face.v2) < alienVertices.size() && 
                static_cast<size_t>(face.v3) < alienVertices.size()) {
                
                const OBJVertex& v1_ref = alienVertices[face.v1];
                const OBJVertex& v2_ref = alienVertices[face.v2];
                const OBJVertex& v3_ref = alienVertices[face.v3];
                
                // Calcular normal
                float nx = (v2_ref.y - v1_ref.y) * (v3_ref.z - v1_ref.z) - (v2_ref.z - v1_ref.z) * (v3_ref.y - v1_ref.y);
                float ny = (v2_ref.z - v1_ref.z) * (v3_ref.x - v1_ref.x) - (v2_ref.x - v1_ref.x) * (v3_ref.z - v1_ref.z);
                float nz = (v2_ref.x - v1_ref.x) * (v3_ref.y - v1_ref.y) - (v2_ref.y - v1_ref.y) * (v3_ref.x - v1_ref.x);
                
                // Normalizar
                float length = sqrt(nx*nx + ny*ny + nz*nz);
                if (length > 0) {
                    face.nx = nx / length;
                    face.ny = ny / length;
                    face.nz = nz / length;
                } else {
                    face.nx = 0.0f;
                    face.ny = 1.0f;
                    face.nz = 0.0f;
                }
            } else {
                face.nx = 0.0f;
                face.ny = 1.0f;
                face.nz = 0.0f;
            }
            
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
    
    // Renderizar o modelo usando triângulos com normais pré-calculadas
    glBegin(GL_TRIANGLES);
    for (const auto& face : alienFaces) {
        if (static_cast<size_t>(face.v1) < alienVertices.size() && 
            static_cast<size_t>(face.v2) < alienVertices.size() && 
            static_cast<size_t>(face.v3) < alienVertices.size()) {
            
            const OBJVertex& v1 = alienVertices[face.v1];
            const OBJVertex& v2 = alienVertices[face.v2];
            const OBJVertex& v3 = alienVertices[face.v3];
            
            // Usar normal pré-calculada
            glNormal3f(face.nx, face.ny, face.nz);
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
            
            // REMOVER: Efeito de energia alien - material energético verde
            // Lighting::setEnergyMaterial(0.3f, 1.0f, 0.3f, 1.5f);
            // glPushMatrix();
            // glScalef(size.x * 0.9f, size.y * 0.9f, size.z * 0.9f);
            // glutSolidCube(1.0f); // <- Este cubo foi removido
            // glPopMatrix();

            break;
            
        case MOVING_VERTICAL:
            // Obstáculo alien móvel - laranja metálico energético
            Lighting::setObstacleMaterial(1.0f, 0.5f, 0.0f, true); // Laranja metálico
            
            // Renderizar modelo do alien (elevado, reduzido e rotacionado)
            glPushMatrix();
            glTranslatef(0.0f, size.y * 1.1f, 0.0f); // Elevar o alien
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f); // Rotacionar 180 graus no eixo Z
            glScalef(size.x * 0.2f, size.y * 0.2f, size.z * 0.2f);
            renderAlienModel();
            glPopMatrix();
            
            break;
            
        case ROCKET:
            // FOGUETE ESPACIAL OTIMIZADO - Vermelho e branco
            
            // Corpo principal - Vermelho brilhante (simplificado)
            Lighting::setObstacleMaterial(0.9f, 0.1f, 0.1f, true);
            glPushMatrix();
            glScalef(size.x * 1.4f, size.y * 1.2f, size.z * 2.5f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Nose cone - Branco polido
            Lighting::setObstacleMaterial(1.0f, 1.0f, 1.0f, true);
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, size.z * 1.25f);
            glScalef(size.x * 0.7f, size.y * 0.7f, size.z * 1.0f);
            glutSolidCone(0.5f, 1.0f, 8, 1);
            glPopMatrix();
            
            // Asas principais - Vermelho escuro (combinadas)
            Lighting::setObstacleMaterial(0.7f, 0.0f, 0.0f, true);
            
            // Asa esquerda
            glPushMatrix();
            glTranslatef(-size.x * 0.8f, 0.0f, -size.z * 0.2f);
            glScalef(size.x * 0.4f, size.y * 0.6f, size.z * 1.2f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Asa direita
            glPushMatrix();
            glTranslatef(size.x * 0.8f, 0.0f, -size.z * 0.2f);
            glScalef(size.x * 0.4f, size.y * 0.6f, size.z * 1.2f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            // Asa estabilizadora superior - Branco
            Lighting::setObstacleMaterial(0.9f, 0.9f, 0.9f, true);
            glPushMatrix();
            glTranslatef(0.0f, size.y * 0.6f, -size.z * 0.2f);
            glScalef(size.x * 0.5f, size.y * 0.2f, size.z * 1.0f);
            glutSolidCube(1.0f);
            glPopMatrix();
            
            break;
            
        case HIGH_OBSTACLE:
            // Obstáculo alien alto - material roxo metálico ameaçador
            Lighting::setObstacleMaterial(0.5f, 0.0f, 0.8f, true); // Roxo metálico escuro
            
            // Renderizar modelo do alien (elevado, reduzido e rotacionado)
            glPushMatrix();
            glTranslatef(0.0f, size.y * 1.1f, 0.0f); // Elevar o alien
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f); // Rotacionar 180 graus no eixo Z
            glScalef(size.x * 0.2f, size.y * 0.2f, size.z * 0.2f);
            renderAlienModel();
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