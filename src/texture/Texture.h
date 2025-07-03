#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <string>
#include <map>

class Texture {
private:
    static std::map<std::string, GLuint> textureMap;
    
public:
    // Inicialização e limpeza
    static void init();
    static void cleanup();
    
    // Carregamento de texturas (preparado para futura implementação)
    static GLuint loadTexture(const std::string& filename);
    static GLuint getTexture(const std::string& name);
    
    // Aplicação de texturas
    static void bindTexture(GLuint textureId);
    static void unbindTexture();
    
    // Configuração de materiais
    static void setMaterial(float r, float g, float b, float alpha = 1.0f);
    static void setMaterialAmbient(float r, float g, float b);
    static void setMaterialDiffuse(float r, float g, float b);
    static void setMaterialSpecular(float r, float g, float b, float shininess);
    
    // Utilidades
    static void enableTexturing();
    static void disableTexturing();
    
private:
    // Placeholder para carregamento de imagens (SOIL/stb_image)
    static unsigned char* loadImageData(const std::string& filename, int& width, int& height, int& channels);
};

#endif 