# Cosmic Dash - Endless Runner Espacial

Um jogo 3D no estilo endless runner desenvolvido em C++ usando OpenGL e GLUT, com jogabilidade em perspectiva 2D lateral.

## 🎮 Sobre o Jogo

Cosmic Dash é um endless runner espacial onde você controla um personagem (cubo) que corre automaticamente em uma pista no espaço cósmico. O objetivo é evitar obstáculos movendo-se entre três faixas e saltando.

### Características

- **Perspectiva 3D** com jogabilidade limitada a um plano 2D
- **Três faixas de movimento** (esquerda, centro, direita)
- **Sistema de pulo** com física realista
- **Obstáculos variados** (estáticos e móveis)
- **Pontuação** baseada em tempo de sobrevivência
- **Sistema de menu** completo
- **HUD informativo** com pontuação e controles
- **Detecção de colisão** AABB precisa

## 🚀 Recursos Implementados

### ✅ Funcionalidades Principais

- [x] Movimento lateral entre 3 faixas
- [x] Sistema de pulo com animação
- [x] Obstáculos com comportamentos diferentes
- [x] Cenário infinito com chão em movimento
- [x] Câmera fixa com perspectiva 3D
- [x] Detecção de colisão AABB
- [x] Sistema de pontuação e high score
- [x] HUD com GLUT Bitmap Font
- [x] Telas de menu, jogo e game over
- [x] Fundo espacial com estrelas

### 🔧 Arquitetura Modular

```
├── main.cpp                    # Loop principal e callbacks
├── src/
│   ├── player/
│   │   ├── Player.h            # Interface do jogador
│   │   └── Player.cpp          # Controle do jogador
│   ├── obstacle/
│   │   ├── Obstacle.h          # Interface dos obstáculos
│   │   └── Obstacle.cpp        # Lógica dos obstáculos
│   ├── scene/
│   │   ├── Scene.h             # Interface do cenário
│   │   └── Scene.cpp           # Cenário e elementos gráficos
│   ├── collision/
│   │   ├── Collision.h         # Interface de colisão
│   │   └── Collision.cpp       # Sistema de colisão AABB
│   ├── score/
│   │   ├── Score.h             # Interface de pontuação
│   │   └── Score.cpp           # Pontuação e HUD
│   ├── menu/
│   │   ├── Menu.h              # Interface de menus
│   │   └── Menu.cpp            # Sistema de menus
│   ├── texture/
│   │   ├── Texture.h           # Interface de texturas
│   │   └── Texture.cpp         # Sistema de texturas (preparado)
│   ├── lighting/
│   │   ├── Lighting.h          # Interface de iluminação
│   │   └── Lighting.cpp        # Sistema de iluminação (preparado)
│   └── utils/
│       └── Vector3.h           # Operações vetoriais 3D
├── Makefile                    # Sistema de compilação
└── README.md                   # Documentação
```

### 🎯 Preparado para Expansão

- **Sistema de texturas** estruturado (placeholders para SOIL/stb_image)
- **Sistema de iluminação** com GL_LIGHT0 e GL_LIGHT1
- **Materiais** configuráveis para objetos
- **Estrutura de spawn** reutilizável para power-ups
- **Sistema de colisão** modular e extensível

## 🛠️ Compilação e Execução

### Dependências

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

#### Fedora/CentOS
```bash
sudo dnf install gcc-c++ mesa-libGL-devel mesa-libGLU-devel freeglut-devel
```

#### Arch Linux
```bash
sudo pacman -S gcc mesa glu freeglut
```

### Compilação

```bash
# Compilar o jogo
make

# Ou compilar e executar diretamente
make run

# Versão debug
make debug

# Versão otimizada
make release

# Verificar dependências
make check-deps
```

### Execução Manual

```bash
g++ -std=c++11 -Wall -Wextra -O2 -I./utils *.cpp -o cosmic_dash -lGL -lGLU -lglut
./cosmic_dash
```

## 🎮 Controles

### No Jogo
- **Setas ← →**: Mover entre faixas
- **Espaço**: Pular
- **ESC**: Pausar/Menu
- **D**: Modo debug (mostrar bounding boxes)

### Nos Menus
- **Setas ↑ ↓**: Navegar opções
- **Enter**: Selecionar
- **ESC**: Voltar/Sair

### Tela de Game Over
- **R**: Reiniciar jogo
- **ESC**: Voltar ao menu

## 🎨 Elementos Visuais

### Jogador
- Cubo azul brilhante representando o personagem
- Movimento suave entre faixas
- Animação de pulo com física realista

### Obstáculos
- **Estáticos** (vermelhos): Obstáculos fixos
- **Móveis** (laranjas): Oscilam verticalmente usando função seno

### Cenário
- Chão metálico com linhas das faixas
- Fundo espacial com gradiente
- Estrelas pontuais para ambientação
- Movimento infinito simulado

## 📊 Sistema de Pontuação

- **10 pontos por segundo** de sobrevivência
- **High score** salvo automaticamente
- **Persistência** em arquivo `highscore.txt`
- **Exibição** em tempo real no HUD

## 🔧 Configurações Técnicas

### Parâmetros Ajustáveis

```cpp
// Player.cpp
const float LANE_WIDTH = 3.0f;          // Largura das faixas
const float JUMP_SPEED = 15.0f;         // Velocidade do pulo
const float GRAVITY = -30.0f;           // Gravidade

// main.cpp
const float OBSTACLE_SPAWN_INTERVAL = 2.0f;  // Intervalo de spawn
const int WINDOW_WIDTH = 800;           // Largura da janela
const int WINDOW_HEIGHT = 600;          // Altura da janela
```

### Câmera
- Posição: (0, 8, 15)
- Alvo: (0, 2, 0)
- FOV: 60°
- Near plane: 0.1
- Far plane: 1000

## 🚀 Expansões Futuras

### Texturas
```cpp
// Exemplo de uso futuro
Texture::loadTexture("player.png");
Texture::bindTexture(playerTexture);
```

### Iluminação
```cpp
// Exemplo de configuração
Lighting::setupLight0(Vector3(10, 15, 10), 0.9f, 0.9f, 1.0f);
Lighting::applyDefaultLighting();
```

### Áudio (Preparado)
- Estrutura para OpenAL ou SDL_mixer
- Sistema de som ambiente e efeitos

## 🐛 Debug e Desenvolvimento

### Modo Debug
- Ative com a tecla **D** durante o jogo
- Mostra bounding boxes de colisão
- Útil para ajustar hitboxes

### Logs
- Inicialização de sistemas
- Carregamento de recursos
- Estados do jogo

## 📝 Licença

Este projeto foi desenvolvido para fins educacionais e demonstração de técnicas de programação em OpenGL/GLUT.

## 🤝 Contribuições

Contribuições são bem-vindas! Áreas de interesse:
- Implementação de texturas
- Sistema de áudio
- Novos tipos de obstáculos
- Power-ups e coletáveis
- Melhorias visuais

## 📞 Suporte

Para problemas de compilação ou execução:
1. Verifique as dependências com `make check-deps`
2. Tente compilar em modo debug: `make debug`
3. Verifique se o OpenGL está funcionando: `glxinfo | grep OpenGL`

---

**Cosmic Dash** - Desenvolvido com ❤️ em C++ e OpenGL 