#ifndef SCENE_H
#define SCENE_H

#include "../utils/Vector3.h"
#include <GL/glut.h>
#include <vector>

struct FloorSection {
    Vector3 position;
    Vector3 size;
    bool active;
    
    FloorSection(Vector3 pos, Vector3 sz) : position(pos), size(sz), active(true) {}
};

class Scene {
private:
    std::vector<FloorSection> floorSections;
    float floorSpeed;
    float floorLength;
    float floorWidth;
    int numSections;
    
    // Estrelas para o fundo
    std::vector<Vector3> stars;
    int numStars;
    
public:
    Scene();
    ~Scene();
    
    void init();
    void update(float deltaTime);
    void render();
    void renderBackground();
    void renderFloor();
    void renderStars();
    
    // Configuração da câmera
    void setupCamera();
    
    // Getters
    float getFloorSpeed() const { return floorSpeed; }
    void setFloorSpeed(float speed) { floorSpeed = speed; }
    
private:
    void generateStars();
    void updateFloorSections(float deltaTime);
};

#endif 