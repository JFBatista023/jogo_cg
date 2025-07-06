#include "Lighting.h"
#include <iostream>

bool Lighting::initialized = false;
Vector3 Lighting::light0Position = Vector3(10.0f, 10.0f, 10.0f);
Vector3 Lighting::light1Position = Vector3(-10.0f, 10.0f, 10.0f);

void Lighting::init() {
    if (initialized) return;
    
    // Habilitar iluminação
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    
    // Configurar sombreamento suave
    glShadeModel(GL_SMOOTH);
    
    // Configurar iluminação ambiente global
    setGlobalAmbient(0.2f, 0.2f, 0.3f);
    
    // Configurar luzes padrão
    setupLight0(light0Position, 1.0f, 1.0f, 1.0f);
    setupLight1(light1Position, 0.5f, 0.5f, 0.8f);
    
    initialized = true;
    std::cout << "Sistema de iluminação inicializado" << std::endl;
}

void Lighting::enable() {
    glEnable(GL_LIGHTING);
}

void Lighting::disable() {
    glDisable(GL_LIGHTING);
}

void Lighting::setupLight0(Vector3 position, float r, float g, float b) {
    light0Position = position;
    
    GLfloat lightPos[] = {position.x, position.y, position.z, 1.0f};
    GLfloat lightColor[] = {r, g, b, 1.0f};
    GLfloat lightAmbient[] = {r * 0.2f, g * 0.2f, b * 0.2f, 1.0f};
    GLfloat lightSpecular[] = {r, g, b, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    // Configurar atenuação
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);
    
    enableLight(GL_LIGHT0);
}

void Lighting::setupLight1(Vector3 position, float r, float g, float b) {
    light1Position = position;
    
    GLfloat lightPos[] = {position.x, position.y, position.z, 1.0f};
    GLfloat lightColor[] = {r, g, b, 1.0f};
    GLfloat lightAmbient[] = {r * 0.1f, g * 0.1f, b * 0.1f, 1.0f};
    GLfloat lightSpecular[] = {r * 0.5f, g * 0.5f, b * 0.5f, 1.0f};
    
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
    
    // Configurar atenuação
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.02f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.002f);
    
    enableLight(GL_LIGHT1);
}

void Lighting::setupAmbientLight(float r, float g, float b) {
    setGlobalAmbient(r, g, b);
}

void Lighting::enableLight(GLenum light) {
    glEnable(light);
}

void Lighting::disableLight(GLenum light) {
    glDisable(light);
}

void Lighting::setLightPosition(GLenum light, Vector3 position) {
    GLfloat lightPos[] = {position.x, position.y, position.z, 1.0f};
    glLightfv(light, GL_POSITION, lightPos);
    
    if (light == GL_LIGHT0) {
        light0Position = position;
    } else if (light == GL_LIGHT1) {
        light1Position = position;
    }
}

void Lighting::setLightColor(GLenum light, float r, float g, float b) {
    GLfloat lightColor[] = {r, g, b, 1.0f};
    glLightfv(light, GL_DIFFUSE, lightColor);
}

void Lighting::setLightAttenuation(GLenum light, float constant, float linear, float quadratic) {
    glLightf(light, GL_CONSTANT_ATTENUATION, constant);
    glLightf(light, GL_LINEAR_ATTENUATION, linear);
    glLightf(light, GL_QUADRATIC_ATTENUATION, quadratic);
}

void Lighting::setGlobalAmbient(float r, float g, float b) {
    GLfloat globalAmbient[] = {r, g, b, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

void Lighting::applyDefaultLighting() {
    if (!initialized) {
        init();
    }
    
    // Aplicar iluminação padrão para o jogo
    enable();
    
    // Luz principal (sol espacial)
    setupLight0(Vector3(10.0f, 15.0f, 10.0f), 0.9f, 0.9f, 1.0f);
    
    // Luz secundária (reflexo das estrelas)
    setupLight1(Vector3(-5.0f, 8.0f, 5.0f), 0.3f, 0.3f, 0.5f);
    
    // Luz ambiente espacial
    setGlobalAmbient(0.1f, 0.1f, 0.2f);
}

void Lighting::initGameLighting() {
    if (!initialized) {
        init();
    }
    
    std::cout << "Inicializando iluminação específica do jogo..." << std::endl;
    
    // Configurar iluminação ambiente espacial equilibrada
    // Ambiente mais claro para garantir visibilidade mas preservar contraste
    setGlobalAmbient(0.25f, 0.25f, 0.35f);
    
    // Luz principal - Luminária central espacial (posicionada acima do jogador)
    Vector3 mainLightPos(0.0f, 25.0f, 10.0f); // Centralizada e mais alta
    setupLight0(mainLightPos, 1.2f, 1.1f, 0.9f); // Luz branca/amarelada intensa
    
    // Luz secundária - Luz de preenchimento (iluminação suave lateral)
    Vector3 fillLightPos(-8.0f, 15.0f, -8.0f);
    setupLight1(fillLightPos, 0.6f, 0.6f, 0.8f); // Luz azulada mais intensa para preenchimento
    
    // Configurar propriedades otimizadas para jogabilidade
    // Luz principal (luminária central) - alcance e intensidade otimizados
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);    // Menor atenuação constante
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.002f);    // Atenuação linear reduzida
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00005f); // Atenuação quadrática mínima
    
    // Luz de preenchimento - suave mas presente
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.8f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.015f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0005f);
    
    enable();
    std::cout << "Iluminação do jogo configurada: ambiente equilibrado + luminária central!" << std::endl;
}

void Lighting::renderLightDebug() {
    // Renderizar posições das luzes para debug
    glDisable(GL_LIGHTING);
    
    // Light 0
    glPushMatrix();
    glTranslatef(light0Position.x, light0Position.y, light0Position.z);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutWireSphere(0.5f, 8, 6);
    glPopMatrix();
    
    // Light 1
    glPushMatrix();
    glTranslatef(light1Position.x, light1Position.y, light1Position.z);
    glColor3f(0.5f, 0.5f, 1.0f);
    glutWireSphere(0.3f, 8, 6);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}

void Lighting::setMaterial(float r, float g, float b, float specularR, float specularG, float specularB, float shininess) {
    GLfloat materialDiffuse[] = {r, g, b, 1.0f};
    GLfloat materialSpecular[] = {specularR, specularG, specularB, 1.0f};
    GLfloat materialAmbient[] = {r * 0.3f, g * 0.3f, b * 0.3f, 1.0f};
    GLfloat materialShininess = shininess;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
}

void Lighting::setMaterial(Vector3 color, Vector3 specular, float shininess) {
    setMaterial(color.x, color.y, color.z, specular.x, specular.y, specular.z, shininess);
}

// Materiais pré-definidos para facilitar uso e garantir consistência

void Lighting::setPlayerMaterial(float r, float g, float b, float intensity) {
    // Material para jogador: altamente reflexivo e brilhante
    setMaterial(r, g, b, intensity, intensity, intensity, 128.0f);
}

void Lighting::setObstacleMaterial(float r, float g, float b, bool isMetallic) {
    if (isMetallic) {
        // Material metálico: reflexão moderada com especular colorido
        setMaterial(r, g, b, r * 0.8f, g * 0.8f, b * 0.8f, 64.0f);
    } else {
        // Material não metálico: reflexão baixa, especular neutro
        setMaterial(r, g, b, 0.3f, 0.3f, 0.3f, 16.0f);
    }
}

void Lighting::setFloorMaterial(float r, float g, float b) {
    // Material para superfícies: moderadamente reflexivo
    setMaterial(r, g, b, 0.6f, 0.6f, 0.7f, 32.0f);
}

void Lighting::setEnergyMaterial(float r, float g, float b, float glow) {
    // Material para elementos energéticos: altamente emissor
    setMaterial(r, g, b, glow, glow, glow, 256.0f);
}