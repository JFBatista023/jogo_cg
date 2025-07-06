#ifndef SCENE_H
#define SCENE_H

#include "../texture/Texture.h"  // Adicionar esta linha
#include <GL/glut.h>

class Scene {
private:
    float floorOffset;
    float floorSpeed;
    
    void renderFloor();
    void renderSkybox();
    void renderLanes();
    
public:
    Scene();
    ~Scene();
    
    void init();
    void update(float deltaTime);
    void render();
    void setupCamera();
    
    void setFloorSpeed(float speed);
    float getFloorSpeed() const;
};

#endif // SCENE_H