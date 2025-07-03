# Estrutura Organizacional do Projeto Cosmic Dash

## 📁 Organização por Módulos

O projeto foi reorganizado seguindo uma estrutura modular profissional, onde cada componente do jogo está em sua própria pasta com arquivos `.h` e `.cpp` correspondentes.

### 🏗️ Estrutura de Diretórios

```
cosmic_dash/
├── main.cpp                    # Ponto de entrada do programa
├── Makefile                    # Sistema de compilação
├── README.md                   # Documentação principal
├── FUNCIONALIDADES.md          # Lista de funcionalidades
├── ESTRUTURA.md               # Este arquivo
├── .gitignore                 # Configuração do Git
├── cosmic_dash                # Executável (gerado)
├── highscore.txt              # Arquivo de high score (gerado)
└── src/                       # Código fonte organizado
    ├── player/
    │   ├── Player.h           # Interface do jogador
    │   └── Player.cpp         # Implementação do jogador
    ├── obstacle/
    │   ├── Obstacle.h         # Interface dos obstáculos
    │   └── Obstacle.cpp       # Implementação dos obstáculos
    ├── scene/
    │   ├── Scene.h            # Interface do cenário
    │   └── Scene.cpp          # Implementação do cenário
    ├── collision/
    │   ├── Collision.h        # Interface de colisão
    │   └── Collision.cpp      # Sistema de detecção de colisão
    ├── score/
    │   ├── Score.h            # Interface de pontuação
    │   └── Score.cpp          # Sistema de pontuação e HUD
    ├── menu/
    │   ├── Menu.h             # Interface de menus
    │   └── Menu.cpp           # Sistema de menus e navegação
    ├── texture/
    │   ├── Texture.h          # Interface de texturas
    │   └── Texture.cpp        # Sistema de texturas (preparado)
    ├── lighting/
    │   ├── Lighting.h         # Interface de iluminação
    │   └── Lighting.cpp       # Sistema de iluminação (preparado)
    └── utils/
        └── Vector3.h          # Estrutura de vetor 3D
```

## 🔗 Dependências entre Módulos

### Hierarquia de Dependências

```
main.cpp
├── player/Player.h
│   └── utils/Vector3.h
├── obstacle/Obstacle.h
│   └── utils/Vector3.h
├── scene/Scene.h
│   └── utils/Vector3.h
├── collision/Collision.h
│   ├── utils/Vector3.h
│   ├── player/Player.h
│   └── obstacle/Obstacle.h
├── score/Score.h
├── menu/Menu.h
├── texture/Texture.h
└── lighting/Lighting.h
    └── utils/Vector3.h
```

### Módulos Base (Sem Dependências)
- **utils/Vector3.h** - Estrutura fundamental para operações 3D
- **score/Score.h** - Sistema independente de pontuação
- **menu/Menu.h** - Sistema independente de menus
- **texture/Texture.h** - Sistema independente de texturas

### Módulos de Entidades (Dependem de utils)
- **player/Player.h** - Jogador (depende de Vector3)
- **obstacle/Obstacle.h** - Obstáculos (depende de Vector3)
- **scene/Scene.h** - Cenário (depende de Vector3)
- **lighting/Lighting.h** - Iluminação (depende de Vector3)

### Módulos de Sistema (Dependem de entidades)
- **collision/Collision.h** - Colisão (depende de Player, Obstacle, Vector3)

## 🎯 Vantagens da Nova Organização

### ✅ Benefícios Implementados

1. **Modularidade**
   - Cada componente em sua própria pasta
   - Separação clara de responsabilidades
   - Facilita manutenção e debugging

2. **Escalabilidade**
   - Fácil adição de novos módulos
   - Estrutura preparada para expansões
   - Reutilização de código facilitada

3. **Organização**
   - Código mais limpo e legível
   - Navegação intuitiva no projeto
   - Padrão profissional de desenvolvimento

4. **Manutenibilidade**
   - Mudanças isoladas por módulo
   - Redução de acoplamento
   - Testes unitários facilitados

5. **Colaboração**
   - Múltiplos desenvolvedores podem trabalhar simultaneamente
   - Conflitos de merge reduzidos
   - Responsabilidades bem definidas

## 🔧 Sistema de Compilação

### Makefile Atualizado

O Makefile foi modificado para trabalhar com a nova estrutura:

```makefile
# Includes apontam para src/
INCLUDES = -I./src

# Sources incluem os caminhos completos
SOURCES = main.cpp \
          src/player/Player.cpp \
          src/obstacle/Obstacle.cpp \
          src/scene/Scene.cpp \
          src/collision/Collision.cpp \
          src/score/Score.cpp \
          src/menu/Menu.cpp \
          src/texture/Texture.cpp \
          src/lighting/Lighting.cpp
```

### Comandos de Compilação

```bash
# Compilar o projeto
make

# Limpar arquivos compilados
make clean

# Compilar e executar
make run

# Versão debug
make debug

# Versão otimizada
make release
```

## 📋 Includes Atualizados

### main.cpp
```cpp
#include "src/player/Player.h"
#include "src/obstacle/Obstacle.h"
#include "src/scene/Scene.h"
#include "src/collision/Collision.h"
#include "src/score/Score.h"
#include "src/menu/Menu.h"
#include "src/texture/Texture.h"
#include "src/lighting/Lighting.h"
```

### Módulos Internos
```cpp
// Em player/Player.h
#include "../utils/Vector3.h"

// Em collision/Collision.h
#include "../utils/Vector3.h"
#include "../player/Player.h"
#include "../obstacle/Obstacle.h"
```

## 🚀 Próximos Passos para Expansão

### Novos Módulos Sugeridos

```
src/
├── audio/              # Sistema de áudio
│   ├── Audio.h
│   └── Audio.cpp
├── physics/            # Física avançada
│   ├── Physics.h
│   └── Physics.cpp
├── effects/            # Efeitos visuais
│   ├── Particle.h
│   └── Particle.cpp
├── ai/                 # Inteligência artificial
│   ├── AI.h
│   └── AI.cpp
└── network/            # Multiplayer (futuro)
    ├── Network.h
    └── Network.cpp
```

### Estrutura de Assets

```
assets/
├── textures/           # Texturas do jogo
├── sounds/             # Efeitos sonoros
├── music/              # Música de fundo
├── models/             # Modelos 3D
└── shaders/            # Shaders customizados
```