#include "Texture.h"
#include <iostream>

std::map<std::string, GLuint> Texture::textureMap;

void Texture::init() {
    // Habilitar texturização
    glEnable(GL_TEXTURE_2D);
    
    // Configurações padrão de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    std::cout << "Sistema de texturas inicializado" << std::endl;
}

void Texture::cleanup() {
    // Liberar todas as texturas carregadas
    for (auto& pair : textureMap) {
        glDeleteTextures(1, &pair.second);
    }
    textureMap.clear();
    
    std::cout << "Sistema de texturas finalizado" << std::endl;
}

GLuint Texture::loadTexture(const std::string& filename) {
    // Verificar se a textura já foi carregada
    auto it = textureMap.find(filename);
    if (it != textureMap.end()) {
        return it->second;
    }
    
    // Placeholder para carregamento futuro com SOIL/stb_image
    GLuint textureId;
    glGenTextures(1, &textureId);
    
    std::cout << "Placeholder: Carregando textura " << filename << std::endl;
    
    // Por enquanto, criar uma textura procedural simples
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Criar uma textura 2x2 de teste
    unsigned char testData[12] = {
        255, 255, 255,  // Branco
        128, 128, 128,  // Cinza
        128, 128, 128,  // Cinza
        255, 255, 255   // Branco
    };
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, testData);
    
    textureMap[filename] = textureId;
    return textureId;
}

GLuint Texture::getTexture(const std::string& name) {
    auto it = textureMap.find(name);
    if (it != textureMap.end()) {
        return it->second;
    }
    return 0; // Textura não encontrada
}

void Texture::bindTexture(GLuint textureId) {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setMaterial(float r, float g, float b, float alpha) {
    GLfloat material[] = {r, g, b, alpha};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
}

void Texture::setMaterialAmbient(float r, float g, float b) {
    GLfloat ambient[] = {r, g, b, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
}

void Texture::setMaterialDiffuse(float r, float g, float b) {
    GLfloat diffuse[] = {r, g, b, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
}

void Texture::setMaterialSpecular(float r, float g, float b, float shininess) {
    GLfloat specular[] = {r, g, b, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void Texture::enableTexturing() {
    glEnable(GL_TEXTURE_2D);
}

void Texture::disableTexturing() {
    glDisable(GL_TEXTURE_2D);
}

unsigned char* Texture::loadImageData(const std::string& filename, int& width, int& height, int& channels) {
    // Placeholder para implementação futura com SOIL ou stb_image
    std::cout << "Placeholder: Carregando dados da imagem " << filename << std::endl;
    
    width = 2;
    height = 2;
    channels = 3;
    
    // Retornar dados de teste
    unsigned char* data = new unsigned char[12];
    for (int i = 0; i < 12; i++) {
        data[i] = (i % 3 == 0) ? 255 : 128; // Padrão simples
    }
    
    return data;
} 