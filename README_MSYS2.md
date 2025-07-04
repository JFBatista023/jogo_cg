# Cosmic Dash - Guia de Execução no MSYS2

## 🚀 Configuração Rápida (Recomendado)

### Método 1: Script Automático

1. **Abra o terminal MSYS2 MINGW64** (importante: não use o MSYS2 comum)
2. **Navegue até o diretório do projeto:**
   ```bash
   cd "/c/Users/socia/Documents/Aulas/8 Periodo/CG/TrabalhoFinal/jogo_cg"
   ```
3. **Execute o script de configuração:**
   ```bash
   bash setup_msys2.sh
   ```

O script irá:
- ✅ Verificar o ambiente MSYS2
- 📦 Instalar dependências automaticamente
- 🔧 Compilar o jogo
- 🎮 Oferecer para executar imediatamente

---

## 🛠️ Configuração Manual

### Passo 1: Instalar Dependências

No terminal **MSYS2 MINGW64**, execute:

```bash
# Atualizar sistema
pacman -Syu

# Instalar compilador
pacman -S mingw-w64-x86_64-gcc

# Instalar make
pacman -S mingw-w64-x86_64-make make

# Instalar OpenGL/GLUT
pacman -S mingw-w64-x86_64-freeglut
pacman -S mingw-w64-x86_64-mesa
```

### Passo 2: Compilar

```bash
# Navegar para o diretório
cd "/c/Users/socia/Documents/Aulas/8 Periodo/CG/TrabalhoFinal/jogo_cg"

# Compilar usando o Makefile para MSYS2
make -f Makefile.msys2

# Ou compilar e executar
make -f Makefile.msys2 run
```

### Passo 3: Executar

```bash
# Executar diretamente
./cosmic_dash.exe

# Ou usar make
make -f Makefile.msys2 run
```

---

## 🎮 Controles do Jogo

### Durante o Jogo
- **Setas ← →**: Mover entre as 3 faixas
- **Espaço**: Pular sobre obstáculos
- **ESC**: Pausar/Menu
- **D**: Ativar modo debug (mostra hitboxes)

### Menus
- **Setas ↑ ↓**: Navegar opções
- **Enter**: Selecionar
- **ESC**: Voltar/Sair

### Game Over
- **R**: Reiniciar jogo
- **ESC**: Voltar ao menu principal

---

## 🔧 Comandos Make Disponíveis

```bash
# Compilação básica
make -f Makefile.msys2

# Compilar e executar
make -f Makefile.msys2 run

# Limpeza
make -f Makefile.msys2 clean

# Versão debug (com símbolos)
make -f Makefile.msys2 debug

# Versão otimizada
make -f Makefile.msys2 release

# Versão estática (recomendada para distribuição)
make -f Makefile.msys2 static

# Verificar dependências
make -f Makefile.msys2 check-deps

# Ajuda
make -f Makefile.msys2 help
```

---

## ⚠️ Possíveis Problemas

### Erro: "g++ not found"
```bash
# Instalar compilador
pacman -S mingw-w64-x86_64-gcc
```

### Erro: "GL/glut.h not found"
```bash
# Instalar GLUT
pacman -S mingw-w64-x86_64-freeglut
```

### Erro: "undefined reference to glutInit"
```bash
# Verificar se está usando o terminal MINGW64
echo $MSYSTEM
# Deve mostrar: MINGW64
```

### Jogo não abre/tela preta
- Verifique se tem drivers gráficos atualizados
- Teste com: `glxinfo` (se disponível)
- Use a versão debug: `make -f Makefile.msys2 debug`

---

## 📁 Estrutura de Arquivos

```
jogo_cg/
├── main.cpp                    # Arquivo principal
├── Makefile                    # Makefile original (Linux)
├── Makefile.msys2             # Makefile para MSYS2 ✨
├── setup_msys2.sh             # Script de configuração ✨
├── README_MSYS2.md            # Este arquivo ✨
└── src/                       # Código fonte modular
    ├── player/
    ├── obstacle/
    ├── scene/
    ├── collision/
    ├── score/
    ├── menu/
    ├── texture/
    ├── lighting/
    └── utils/
```

---

## 🎯 Objetivo do Jogo

**Cosmic Dash** é um endless runner espacial onde você:

1. **Controla um cubo azul** que corre automaticamente
2. **Move entre 3 faixas** para desviar de obstáculos
3. **Pula** sobre obstáculos quando necessário
4. **Sobrevive** o máximo de tempo possível
5. **Acumula pontos** (10 por segundo)

### Obstáculos
- 🔴 **Vermelhos**: Estáticos
- 🟠 **Laranjas**: Movem verticalmente

### HUD
- Pontuação atual
- High score
- Controles

---

## 🚀 Próximos Passos

Após compilar com sucesso, você pode:

1. **Jogar** e testar todas as funcionalidades
2. **Explorar o código** para entender a estrutura
3. **Modificar** parâmetros no código (velocidade, dificuldade)
4. **Expandir** com novas funcionalidades

---

**Divirta-se jogando Cosmic Dash! 🎮✨**
