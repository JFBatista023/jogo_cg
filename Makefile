# Makefile para Cosmic Dash
# Jogo 3D Endless Runner em OpenGL/GLUT

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
INCLUDES = -I./src

# Bibliotecas
LIBS = -lGL -lGLU -lglut -lfmod

# Arquivos fonte
SOURCES = main.cpp \
          src/player/Player.cpp \
          src/obstacle/Obstacle.cpp \
          src/scene/Scene.cpp \
          src/collision/Collision.cpp \
          src/score/Score.cpp \
          src/menu/Menu.cpp \
          src/texture/Texture.cpp \
          src/lighting/Lighting.cpp \
          src/audio/Audio.cpp

# Arquivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Nome do executável
TARGET = cosmic_dash

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "Cosmic Dash compilado com sucesso!"
	@echo "Execute com: ./$(TARGET)"

# Regra para compilar arquivos .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Arquivos limpos!"

# Executar o jogo
run: $(TARGET)
	./$(TARGET)

# Instalar dependências (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libfmod-dev

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# Verificar dependências
check-deps:
	@echo "Verificando dependências..."
	@pkg-config --exists gl || echo "OpenGL não encontrado!"
	@pkg-config --exists glu || echo "GLU não encontrado!"
	@ldconfig -p | grep glut > /dev/null || echo "GLUT não encontrado!"
	@ldconfig -p | grep fmod > /dev/null || echo "FMOD não encontrado! Execute: sudo apt-get install libfmod-dev"
	@echo "Verificação concluída."

# Ajuda
help:
	@echo "Cosmic Dash - Makefile"
	@echo "======================"
	@echo "Comandos disponíveis:"
	@echo "  make          - Compilar o jogo"
	@echo "  make run      - Compilar e executar"
	@echo "  make clean    - Limpar arquivos compilados"
	@echo "  make debug    - Compilar versão debug"
	@echo "  make release  - Compilar versão otimizada"
	@echo "  make install-deps - Instalar dependências (Ubuntu/Debian)"
	@echo "  make check-deps   - Verificar dependências"
	@echo "  make help     - Mostrar esta ajuda"

# Evitar conflitos com arquivos de mesmo nome
.PHONY: all clean run install-deps debug release check-deps help 