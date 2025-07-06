#include "Texture.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring>

std::map<std::string, GLuint> Texture::textures;
bool Texture::initialized = false;

bool Texture::init() {
    if (initialized) return true;
    
    std::cout << "Inicializando sistema de texturas..." << std::endl;
    
    // Habilitar texturas 2D
    glEnable(GL_TEXTURE_2D);
    
    // Criar texturas procedurais padrão
    createGalaxyTexture("galaxy");
    createStarFieldTexture("starfield");
    createNebulaTexture("nebula");
    
    initialized = true;
    std::cout << "Sistema de texturas inicializado com " << textures.size() << " texturas." << std::endl;
    
    return true;
}

void Texture::cleanup() {
    for (auto& pair : textures) {
        glDeleteTextures(1, &pair.second);
    }
    textures.clear();
    initialized = false;
    std::cout << "Sistema de texturas finalizado." << std::endl;
}

GLuint Texture::loadTexture(const std::string& name, const std::string& filename) {
    // Verificar se já foi carregada
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }
    
    int width, height;
    unsigned char* data = loadBMP(filename, width, height);
    
    if (!data) {
        std::cout << "Falha ao carregar textura: " << filename << std::endl;
        return 0;
    }
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Configurar parâmetros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Carregar dados da textura (usando GL_RGB em vez de GL_BGR)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    delete[] data;
    
    textures[name] = textureId;
    std::cout << "Textura carregada: " << name << " (" << filename << ")" << std::endl;
    
    return textureId;
}

GLuint Texture::createGalaxyTexture(const std::string& name, int width, int height) {
    // Verificar se já foi criada
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }
    
    std::cout << "Criando textura de galáxia procedural..." << std::endl;
    
    unsigned char* data = new unsigned char[width * height * 3];
    
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float maxRadius = std::min(centerX, centerY);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            
            // Calcular distância do centro
            float dx = x - centerX;
            float dy = y - centerY;
            float distance = sqrt(dx * dx + dy * dy);
            float normalizedDistance = distance / maxRadius;
            
            // Calcular ângulo para espiral
            float angle = atan2(dy, dx);
            float spiral = sin(angle * 3.0f + distance * 0.1f) * 0.5f + 0.5f;
            
            // Criar gradiente radial
            float intensity = 1.0f - normalizedDistance;
            intensity = std::max(0.0f, std::min(1.0f, intensity));
            
            // Adicionar ruído para variação
            float noise = (rand() % 100) / 100.0f * 0.3f;
            intensity += noise;
            intensity = std::max(0.0f, std::min(1.0f, intensity));
            
            // Combinar com espiral
            intensity *= spiral * 0.7f + 0.3f;
            
            // Cores da galáxia (tons de azul, roxo e branco)
            if (normalizedDistance < 0.3f) {
                // Centro - branco/amarelo
                data[index] = (unsigned char)(255 * intensity);     // R
                data[index + 1] = (unsigned char)(240 * intensity); // G
                data[index + 2] = (unsigned char)(200 * intensity); // B
            } else if (normalizedDistance < 0.7f) {
                // Meio - azul/ciano
                data[index] = (unsigned char)(100 * intensity);     // R
                data[index + 1] = (unsigned char)(150 * intensity); // G
                data[index + 2] = (unsigned char)(255 * intensity); // B
            } else {
                // Borda - roxo/escuro
                data[index] = (unsigned char)(80 * intensity);      // R
                data[index + 1] = (unsigned char)(50 * intensity);  // G
                data[index + 2] = (unsigned char)(120 * intensity); // B
            }
        }
    }
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Configurar parâmetros
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    delete[] data;
    
    textures[name] = textureId;
    std::cout << "Textura de galáxia criada: " << name << std::endl;
    
    return textureId;
}

GLuint Texture::createStarFieldTexture(const std::string& name, int width, int height) {
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }
    
    std::cout << "Criando textura de campo de estrelas..." << std::endl;
    
    unsigned char* data = new unsigned char[width * height * 3];
    
    // Preencher com preto
    memset(data, 0, width * height * 3);
    
    // Adicionar estrelas aleatórias
    int numStars = (width * height) / 1000; // Densidade de estrelas
    
    for (int i = 0; i < numStars; i++) {
        int x = rand() % width;
        int y = rand() % height;
        int index = (y * width + x) * 3;
        
        // Intensidade aleatória da estrela
        float intensity = 0.5f + (rand() % 100) / 200.0f;
        unsigned char starColor = (unsigned char)(255 * intensity);
        
        data[index] = starColor;     // R
        data[index + 1] = starColor; // G
        data[index + 2] = starColor; // B
        
        // Adicionar brilho ao redor (opcional)
        if (rand() % 10 == 0) { // 10% das estrelas têm brilho
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        int nindex = (ny * width + nx) * 3;
                        unsigned char glow = (unsigned char)(100 * intensity);
                        data[nindex] = std::max(data[nindex], glow);
                        data[nindex + 1] = std::max(data[nindex + 1], glow);
                        data[nindex + 2] = std::max(data[nindex + 2], glow);
                    }
                }
            }
        }
    }
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    delete[] data;
    
    textures[name] = textureId;
    std::cout << "Textura de campo de estrelas criada: " << name << std::endl;
    
    return textureId;
}

GLuint Texture::createNebulaTexture(const std::string& name, int width, int height) {
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }
    
    std::cout << "Criando textura de nebulosa..." << std::endl;
    
    unsigned char* data = new unsigned char[width * height * 3];
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            
            // Criar padrão de nebulosa com múltiplas camadas de ruído
            float nx = (float)x / width * 4.0f;
            float ny = (float)y / height * 4.0f;
            
            float noise1 = sin(nx * 3.14159f) * cos(ny * 3.14159f);
            float noise2 = sin(nx * 6.28318f) * cos(ny * 6.28318f) * 0.5f;
            float noise3 = (rand() % 100) / 500.0f;
            
            float intensity = (noise1 + noise2 + noise3) * 0.5f + 0.5f;
            intensity = std::max(0.0f, std::min(1.0f, intensity));
            
            // Cores de nebulosa (rosa, roxo, azul)
            data[index] = (unsigned char)(120 * intensity + 30);     // R
            data[index + 1] = (unsigned char)(80 * intensity + 20);  // G
            data[index + 2] = (unsigned char)(160 * intensity + 40); // B
        }
    }
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    delete[] data;
    
    textures[name] = textureId;
    std::cout << "Textura de nebulosa criada: " << name << std::endl;
    
    return textureId;
}

GLuint Texture::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    return 0;
}

void Texture::bind(const std::string& name) {
    GLuint textureId = getTexture(name);
    if (textureId != 0) {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
}

void Texture::bind(GLuint textureId) {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::exists(const std::string& name) {
    return textures.find(name) != textures.end();
}

unsigned char* Texture::loadBMP(const std::string& filename, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Erro: Não foi possível abrir arquivo " << filename << std::endl;
        return nullptr;
    }
    
    // Ler header BMP (implementação simplificada)
    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);
    
    // Verificar se é um arquivo BMP válido
    if (header[0] != 'B' || header[1] != 'M') {
        std::cout << "Erro: " << filename << " não é um arquivo BMP válido" << std::endl;
        return nullptr;
    }
    
    // Extrair informações do header
    width = *(int*)&header[18];
    height = *(int*)&header[22];
    
    // Calcular tamanho dos dados
    int dataSize = width * height * 3;
    unsigned char* data = new unsigned char[dataSize];
    
    // Ler dados da imagem
    file.read(reinterpret_cast<char*>(data), dataSize);
    file.close();
    
    // Converter BGR para RGB se necessário (BMP usa BGR)
    for (int i = 0; i < dataSize; i += 3) {
        unsigned char temp = data[i];     // B
        data[i] = data[i + 2];           // R
        data[i + 2] = temp;              // B
    }
    
    std::cout << "BMP carregado: " << filename << " (" << width << "x" << height << ")" << std::endl;
    
    return data;
}