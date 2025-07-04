#!/bin/bash
# Script de configuração automática para Cosmic Dash no MSYS2
# Execute este script no terminal MSYS2 MINGW64

echo "=== Configuração Automática - Cosmic Dash MSYS2 ==="
echo ""

# Verificar se estamos no MSYS2
if [[ ! "$MSYSTEM" =~ ^MINGW ]]; then
    echo "❌ ERRO: Execute este script no terminal MSYS2 MINGW64!"
    echo "   Abra o 'MSYS2 MINGW64' e tente novamente."
    exit 1
fi

echo "✅ Ambiente MSYS2 detectado: $MSYSTEM"
echo ""

# Função para verificar se um pacote está instalado
check_package() {
    if pacman -Qi $1 &> /dev/null; then
        echo "✅ $1 já está instalado"
        return 0
    else
        echo "❌ $1 não está instalado"
        return 1
    fi
}

# Verificar dependências
echo "🔍 Verificando dependências..."
packages_needed=()

if ! check_package "mingw-w64-x86_64-gcc"; then
    packages_needed+=("mingw-w64-x86_64-gcc")
fi

if ! check_package "mingw-w64-x86_64-make"; then
    packages_needed+=("mingw-w64-x86_64-make")
fi

if ! check_package "mingw-w64-x86_64-freeglut"; then
    packages_needed+=("mingw-w64-x86_64-freeglut")
fi

if ! check_package "mingw-w64-x86_64-mesa"; then
    packages_needed+=("mingw-w64-x86_64-mesa")
fi

# Instalar pacotes necessários
if [ ${#packages_needed[@]} -gt 0 ]; then
    echo ""
    echo "📦 Instalando pacotes necessários..."
    echo "Pacotes a instalar: ${packages_needed[*]}"
    echo ""
    
    read -p "Deseja instalar os pacotes necessários? (s/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Ss]$ ]]; then
        pacman -S --needed ${packages_needed[*]}
    else
        echo "❌ Instalação cancelada. Execute manualmente:"
        echo "   pacman -S ${packages_needed[*]}"
        exit 1
    fi
else
    echo "✅ Todas as dependências estão instaladas!"
fi

echo ""
echo "🔧 Verificando compilador..."
if command -v g++ &> /dev/null; then
    echo "✅ g++ encontrado: $(g++ --version | head -n1)"
else
    echo "❌ g++ não encontrado!"
    exit 1
fi

echo ""
echo "🔧 Verificando make..."
if command -v make &> /dev/null; then
    echo "✅ make encontrado: $(make --version | head -n1)"
else
    echo "❌ make não encontrado!"
    exit 1
fi

echo ""
echo "🎮 Compilando Cosmic Dash..."
if [ -f "Makefile.msys2" ]; then
    make -f Makefile.msys2 clean
    make -f Makefile.msys2
    
    if [ -f "cosmic_dash.exe" ]; then
        echo ""
        echo "🎉 Cosmic Dash compilado com sucesso!"
        echo ""
        echo "📋 Como executar:"
        echo "   ./cosmic_dash.exe"
        echo "   ou"
        echo "   make -f Makefile.msys2 run"
        echo ""
        echo "🎮 Controles do jogo:"
        echo "   ← → : Mover entre faixas"
        echo "   Espaço : Pular"
        echo "   ESC : Menu/Pausar"
        echo "   D : Modo debug"
        echo ""
        
        read -p "Deseja executar o jogo agora? (s/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Ss]$ ]]; then
            echo "🚀 Iniciando Cosmic Dash..."
            ./cosmic_dash.exe
        fi
    else
        echo "❌ Erro na compilação!"
        exit 1
    fi
else
    echo "❌ Makefile.msys2 não encontrado!"
    echo "   Certifique-se de estar no diretório correto do projeto."
    exit 1
fi

echo ""
echo "✅ Configuração concluída!"
