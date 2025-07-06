#ifndef LIGHTING_H
#define LIGHTING_H

#include <GL/glut.h>
#include "../utils/Vector3.h"

class Lighting {
public:
    // Inicialização e configuração
    static void init();
    static void initGameLighting(); // Nova função para iluminação específica do jogo
    static void enable();
    static void disable();
    
    // Configuração de luzes
    static void setupLight0(Vector3 position, float r, float g, float b);
    static void setupLight1(Vector3 position, float r, float g, float b);
    static void setupAmbientLight(float r, float g, float b);
    
    // Controle de luzes individuais
    static void enableLight(GLenum light);
    static void disableLight(GLenum light);
    
    // Configuração de propriedades da luz
    static void setLightPosition(GLenum light, Vector3 position);
    static void setLightColor(GLenum light, float r, float g, float b);
    static void setLightAttenuation(GLenum light, float constant, float linear, float quadratic);
    
    // Configuração de iluminação ambiente
    static void setGlobalAmbient(float r, float g, float b);
    
    // Utilidades
    static void applyDefaultLighting();
    static void renderLightDebug();
    
    // Utilidades para materiais
    static void setMaterial(float r, float g, float b, float specularR = 1.0f, float specularG = 1.0f, float specularB = 1.0f, float shininess = 32.0f);
    static void setMaterial(Vector3 color, Vector3 specular = Vector3(1.0f, 1.0f, 1.0f), float shininess = 32.0f);
    
    // Materiais pré-definidos para facilitar uso e garantir consistência
    static void setPlayerMaterial(float r, float g, float b, float intensity = 1.0f);
    static void setObstacleMaterial(float r, float g, float b, bool isMetallic = true);
    static void setFloorMaterial(float r, float g, float b);
    static void setEnergyMaterial(float r, float g, float b, float glow = 2.0f);
    
private:
    static bool initialized;
    static Vector3 light0Position;
    static Vector3 light1Position;
};

#endif