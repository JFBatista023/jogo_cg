#ifndef LIGHTING_H
#define LIGHTING_H

#include <GL/glut.h>
#include "../utils/Vector3.h"

class Lighting {
public:
    // Inicialização e configuração
    static void init();
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
    
private:
    static bool initialized;
    static Vector3 light0Position;
    static Vector3 light1Position;
};

#endif 