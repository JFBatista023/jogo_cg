#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <string>
#include <map>

class Texture {
private:
    static std::map<std::string, GLuint> textures;
    static bool initialized;
    
    // Função auxiliar para carregar imagem BMP
    static unsigned char* loadBMP(const std::string& filename, int& width, int& height);
    
public:
    static bool init();
    static void cleanup();
    
    // Carregar textura de arquivo
    static GLuint loadTexture(const std::string& name, const std::string& filename);
    
    // Criar texturas procedurais
    static GLuint createGalaxyTexture(const std::string& name, int width = 512, int height = 512);
    static GLuint createStarFieldTexture(const std::string& name, int width = 512, int height = 512);
    static GLuint createNebulaTexture(const std::string& name, int width = 512, int height = 512);
    
    // Obter textura carregada
    static GLuint getTexture(const std::string& name);
    
    // Bind textura
    static void bind(const std::string& name);
    static void bind(GLuint textureId);
    static void unbind();
    
    // Verificar se textura existe
    static bool exists(const std::string& name);
};

#endif // TEXTURE_H