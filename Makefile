# Makefile para Cosmic Dash
# Jogo 3D Endless Runner em OpenGL/GLUT

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
INCLUDES = -I./src

# Configurações específicas por OS
ifeq ($(DETECTED_OS),Windows)
    # Windows
    LIBS = -lopengl32 -lglu32 -lfreeglut -lwinmm
    CXXFLAGS += -DWINDOWS_AUDIO
    TARGET = cosmic_dash.exe
    CLEAN_CMD = del /Q
    PATH_SEP = \\
    EXEC_PREFIX = 
else
    # Linux/Unix
    LIBS = -lGL -lGLU -lglut
    TARGET = cosmic_dash
    CLEAN_CMD = rm -f
    PATH_SEP = /
    EXEC_PREFIX = ./
    
    # Verificar se OpenAL está disponível no Linux
    OPENAL_AVAILABLE := $(shell pkg-config --exists openal 2>/dev/null && echo "yes" || echo "no")
    SNDFILE_AVAILABLE := $(shell pkg-config --exists sndfile 2>/dev/null && echo "yes" || echo "no")
    
    # Adicionar bibliotecas de áudio se disponíveis
    ifeq ($(OPENAL_AVAILABLE),yes)
        LIBS += -lopenal
        CXXFLAGS += -DOPENAL_AVAILABLE
    endif
    
    ifeq ($(SNDFILE_AVAILABLE),yes)
        LIBS += -lsndfile
        CXXFLAGS += -DSNDFILE_AVAILABLE
    endif
endif

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

# Regra principal
all: check-audio $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "Cosmic Dash compilado com sucesso para $(DETECTED_OS)!"
	@echo "Execute com: $(EXEC_PREFIX)$(TARGET)"

# Regra para compilar arquivos .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Verificar bibliotecas de áudio disponíveis
check-audio:
	@echo "=== Verificando bibliotecas de áudio ($(DETECTED_OS)) ==="
ifeq ($(DETECTED_OS),Windows)
	@echo "✓ Windows Multimedia API (winmm) disponível"
else
	@if pkg-config --exists openal; then \
		echo "✓ OpenAL encontrado"; \
	else \
		echo "✗ OpenAL não encontrado"; \
	fi
	@if pkg-config --exists sndfile; then \
		echo "✓ libsndfile encontrado"; \
	else \
		echo "✗ libsndfile não encontrado"; \
	fi
	@if command -v aplay >/dev/null 2>&1; then \
		echo "✓ ALSA (aplay) disponível"; \
	else \
		echo "✗ ALSA não disponível"; \
	fi
	@if command -v paplay >/dev/null 2>&1; then \
		echo "✓ PulseAudio (paplay) disponível"; \
	else \
		echo "✗ PulseAudio não disponível"; \
	fi
endif
	@echo "====================================="

# Limpeza
clean:
ifeq ($(DETECTED_OS),Windows)
	if exist *.o del /Q *.o
	if exist src\player\*.o del /Q src\player\*.o
	if exist src\obstacle\*.o del /Q src\obstacle\*.o
	if exist src\scene\*.o del /Q src\scene\*.o
	if exist src\collision\*.o del /Q src\collision\*.o
	if exist src\score\*.o del /Q src\score\*.o
	if exist src\menu\*.o del /Q src\menu\*.o
	if exist src\texture\*.o del /Q src\texture\*.o
	if exist src\lighting\*.o del /Q src\lighting\*.o
	if exist src\audio\*.o del /Q src\audio\*.o
	if exist $(TARGET) del /Q $(TARGET)
else
	$(CLEAN_CMD) $(OBJECTS) $(TARGET)
endif
	@echo "Arquivos limpos!"

# Executar o jogo
run: $(TARGET)
	$(EXEC_PREFIX)$(TARGET)

# Instalar dependências
install-deps:
ifeq ($(DETECTED_OS),Windows)
	@echo "Para Windows, instale:"
	@echo "1. MinGW-w64 ou MSYS2"
	@echo "2. FreeGLUT para Windows"
	@echo "3. OpenGL libraries (geralmente já incluídas)"
else
	sudo apt-get update
	sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
	sudo apt-get install libopenal-dev libsndfile1-dev alsa-utils pulseaudio-utils
endif

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# Verificar dependências
check-deps:
	@echo "Verificando dependências para $(DETECTED_OS)..."
ifeq ($(DETECTED_OS),Windows)
	@echo "Sistema: Windows"
	@if exist "C:\Windows\System32\opengl32.dll" (echo "✓ OpenGL disponível") else (echo "✗ OpenGL não encontrado")
else
	@pkg-config --exists gl || echo "OpenGL não encontrado!"
	@pkg-config --exists glu || echo "GLU não encontrado!"
	@ldconfig -p | grep glut > /dev/null || echo "GLUT não encontrado!"
endif
	@echo "Verificação concluída."

# Ajuda
help:
	@echo "Cosmic Dash - Makefile ($(DETECTED_OS))"
	@echo "========================================"
	@echo "Comandos disponíveis:"
	@echo "  make              - Compilar o jogo"
	@echo "  make run          - Compilar e executar"
	@echo "  make clean        - Limpar arquivos compilados"
	@echo "  make debug        - Compilar versão debug"
	@echo "  make release      - Compilar versão otimizada"
	@echo "  make install-deps - Instalar dependências"
	@echo "  make check-audio  - Verificar bibliotecas de áudio"
	@echo "  make check-deps   - Verificar dependências"

.PHONY: all clean run install-deps debug release check-deps check-audio help