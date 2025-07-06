#!/bin/bash

echo "=== Instalação do FMOD para Cosmic Dash ==="
echo ""

# Verificar se já está instalado
if pkg-config --exists fmod; then
    echo "FMOD já está instalado!"
    exit 0
fi

echo "FMOD não encontrado. Instalando do arquivo local..."

# Verificar se o arquivo .tar.gz existe
if [ ! -f "fmodstudioapi20308linux.tar.gz" ]; then
    echo "Erro: Arquivo fmodstudioapi20308linux.tar.gz não encontrado!"
    echo "Certifique-se de que o arquivo está no diretório atual."
    exit 1
fi

echo "Extraindo FMOD..."
tar -xzf fmodstudioapi20308linux.tar.gz

# Encontrar o diretório extraído
FMOD_DIR=$(find . -maxdepth 1 -type d -name "fmodstudioapi*" | head -n 1)

if [ -z "$FMOD_DIR" ]; then
    echo "Erro: Não foi possível encontrar o diretório do FMOD extraído!"
    exit 1
fi

echo "Instalando FMOD de: $FMOD_DIR"

# Criar diretórios se não existirem
sudo mkdir -p /usr/local/include
sudo mkdir -p /usr/local/lib

# Copiar headers
echo "Copiando headers..."
sudo cp -r "$FMOD_DIR/api/core/inc/"* /usr/local/include/

# Copiar bibliotecas
echo "Copiando bibliotecas..."
sudo cp "$FMOD_DIR/api/core/lib/x86_64/"* /usr/local/lib/

# Atualizar cache de bibliotecas
echo "Atualizando cache de bibliotecas..."
sudo ldconfig

# Criar arquivo pkg-config se não existir
if [ ! -f "/usr/local/lib/pkgconfig/fmod.pc" ]; then
    echo "Criando arquivo pkg-config..."
    sudo mkdir -p /usr/local/lib/pkgconfig
    sudo tee /usr/local/lib/pkgconfig/fmod.pc > /dev/null <<EOF
prefix=/usr/local
exec_prefix=\${prefix}
libdir=\${exec_prefix}/lib
includedir=\${prefix}/include

Name: fmod
Description: FMOD Studio API
Version: 2.03.08
Libs: -L\${libdir} -lfmod
Cflags: -I\${includedir}
EOF
fi

# Verificar se a instalação foi bem-sucedida
if pkg-config --exists fmod; then
    echo "FMOD instalado com sucesso!"
    echo "Você pode agora compilar o projeto com 'make'"
else
    echo "Aviso: FMOD pode não estar sendo detectado pelo pkg-config"
    echo "Mas os arquivos foram instalados em /usr/local/"
    echo "Tente compilar com 'make' mesmo assim"
fi

# Limpar arquivos temporários
echo "Limpando arquivos temporários..."
rm -rf "$FMOD_DIR"

echo "Instalação concluída!" 