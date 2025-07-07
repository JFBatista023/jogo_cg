#include "Menu.h"
#include <cmath>

Menu::Menu() {
    currentState = MENU;
    selectedOption = 0;
    numMenuOptions = 2; // Jogar, Sair
}

Menu::~Menu() {
}

void Menu::render() {
    switch (currentState) {
        case MENU:
            renderMainMenu();
            break;
        case GAME_OVER:
            renderGameOverMenu();
            break;
        case PAUSED:
            renderPauseMenu();
            break;
        default:
            break;
    }
}

void Menu::renderMainMenu() {
    // Limpar o buffer de cor primeiro
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Configurar para renderização 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    // Fundo espacial escuro com gradiente
    renderSpaceBackground();
    
    // Partículas animadas
    renderAnimatedParticles();
    
    // Painel principal do menu
    renderMainMenuPanel();
    
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Menu::renderSpaceBackground() {
    // Gradiente espacial escuro
    glBegin(GL_QUADS);
    // Topo - azul escuro espacial
    glColor3f(0.05f, 0.05f, 0.15f);
    glVertex2f(0, 600);
    glVertex2f(800, 600);
    // Meio - roxo escuro
    glColor3f(0.1f, 0.05f, 0.2f);
    glVertex2f(800, 300);
    glVertex2f(0, 300);
    // Base - preto profundo
    glColor3f(0.02f, 0.02f, 0.08f);
    glVertex2f(800, 0);
    glVertex2f(0, 0);
    glEnd();
    
    // Campo de estrelas dinâmico
    renderStarField();
    
    // Nebulosas sutis
    renderNebulas();
}

void Menu::renderStarField() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Estrelas grandes brilhantes
    for (int i = 0; i < 50; i++) {
        float x = (i * 73 + 123) % 800;
        float y = (i * 47 + 200) % 600;
        float brightness = 0.7f + 0.3f * sin(i * 0.5f);
        
        glColor4f(1.0f, 1.0f, 1.0f, brightness);
        glPointSize(3.0f + sin(i * 0.3f));
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    }
    
    // Estrelas pequenas
    for (int i = 0; i < 150; i++) {
        float x = (i * 127 + 50) % 800;
        float y = (i * 83 + 100) % 600;
        float brightness = 0.3f + 0.2f * sin(i * 0.7f);
        
        glColor4f(0.8f, 0.9f, 1.0f, brightness);
        glPointSize(1.0f + sin(i * 0.2f) * 0.5f);
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    }
    
    glDisable(GL_BLEND);
}

void Menu::renderNebulas() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Nebulosa azul no canto superior direito
    glColor4f(0.2f, 0.4f, 0.8f, 0.1f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float angle = i * 2.0f * 3.14159f / 20.0f;
        float radius = 150.0f + 50.0f * sin(i * 0.5f);
        float x = 650 + radius * cos(angle);
        float y = 450 + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    // Nebulosa roxa no canto inferior esquerdo
    glColor4f(0.6f, 0.2f, 0.8f, 0.08f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 20; i++) {
        float angle = i * 2.0f * 3.14159f / 20.0f;
        float radius = 120.0f + 30.0f * sin(i * 0.7f);
        float x = 150 + radius * cos(angle);
        float y = 150 + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    glDisable(GL_BLEND);
}

void Menu::renderAnimatedParticles() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Partículas flutuantes
    for (int i = 0; i < 20; i++) {
        float time = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
        float x = 100 + (i * 35) % 600 + 20.0f * sin(time + i * 0.5f);
        float y = 50 + (i * 43) % 500 + 15.0f * cos(time * 0.7f + i * 0.3f);
        float alpha = 0.3f + 0.2f * sin(time * 2.0f + i);
        
        glColor4f(0.4f, 0.8f, 1.0f, alpha);
        glPointSize(2.0f + sin(time + i) * 0.5f);
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    }
    
    glDisable(GL_BLEND);
}

void Menu::renderMainMenuPanel() {
    // Painel principal translúcido
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Fundo do painel
    glColor4f(0.05f, 0.1f, 0.2f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(150, 150);
    glVertex2f(650, 150);
    glVertex2f(650, 500);
    glVertex2f(150, 500);
    glEnd();
    
    // Borda brilhante
    glColor4f(0.3f, 0.7f, 1.0f, 0.9f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(150, 150);
    glVertex2f(650, 150);
    glVertex2f(650, 500);
    glVertex2f(150, 500);
    glEnd();
    
    glDisable(GL_BLEND);
    
    // Título principal com efeito brilhante
    renderEnhancedTitle();
    
    // Opções do menu com estilo moderno
    renderModernMenuOptions();
    
    // Instruções estilizadas
    renderStylizedInstructions();
}

void Menu::renderEnhancedTitle() {
    // Efeito de brilho atrás do título
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    float time = glutGet(GLUT_ELAPSED_TIME) * 0.003f;
    float glow = 0.3f + 0.2f * sin(time);
    
    glColor4f(0.2f, 0.6f, 1.0f, glow);
    glBegin(GL_QUADS);
    glVertex2f(200, 420);
    glVertex2f(600, 420);
    glVertex2f(600, 460);
    glVertex2f(200, 460);
    glEnd();
    
    glDisable(GL_BLEND);
    
    // Título principal
    glColor3f(1.0f, 1.0f, 1.0f);
    renderTextCentered(440, "COSMIC DASH", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Subtítulo
    glColor3f(0.7f, 0.9f, 1.0f);
    renderTextCentered(410, "Endless Runner Espacial", GLUT_BITMAP_HELVETICA_18);
}

void Menu::renderModernMenuOptions() {
    float optionY[] = {340, 290};
    std::string options[] = {"JOGAR", "SAIR"};
    
    for (int i = 0; i < 2; i++) {
        bool isSelected = (selectedOption == i);
        
        // Fundo da opção
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        if (isSelected) {
            // Opção selecionada - brilho animado
            float time = glutGet(GLUT_ELAPSED_TIME) * 0.005f;
            float pulse = 0.7f + 0.3f * sin(time * 3.0f);
            
            glColor4f(0.2f, 0.8f, 1.0f, pulse * 0.6f);
            glBegin(GL_QUADS);
            glVertex2f(250, optionY[i] - 15);
            glVertex2f(550, optionY[i] - 15);
            glVertex2f(550, optionY[i] + 25);
            glVertex2f(250, optionY[i] + 25);
            glEnd();
            
            // Borda brilhante
            glColor4f(1.0f, 1.0f, 1.0f, pulse);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(250, optionY[i] - 15);
            glVertex2f(550, optionY[i] - 15);
            glVertex2f(550, optionY[i] + 25);
            glVertex2f(250, optionY[i] + 25);
            glEnd();
        } else {
            // Opção não selecionada
            glColor4f(0.1f, 0.2f, 0.4f, 0.5f);
            glBegin(GL_QUADS);
            glVertex2f(270, optionY[i] - 10);
            glVertex2f(530, optionY[i] - 10);
            glVertex2f(530, optionY[i] + 20);
            glVertex2f(270, optionY[i] + 20);
            glEnd();
        }
        
        glDisable(GL_BLEND);
        
        // Texto da opção
        if (isSelected) {
            glColor3f(1.0f, 1.0f, 1.0f);
            renderTextCentered(optionY[i], "► " + options[i] + " ◄", GLUT_BITMAP_HELVETICA_18);
        } else {
            glColor3f(0.7f, 0.8f, 0.9f);
            renderTextCentered(optionY[i], options[i], GLUT_BITMAP_HELVETICA_18);
        }
    }
}

void Menu::renderStylizedInstructions() {
    // Painel de instruções
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(0.02f, 0.05f, 0.1f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(180, 180);
    glVertex2f(620, 180);
    glVertex2f(620, 250);
    glVertex2f(180, 250);
    glEnd();
    
    glDisable(GL_BLEND);
    
    // Título das instruções
    glColor3f(0.8f, 0.9f, 1.0f);
    renderTextCentered(235, "CONTROLES", GLUT_BITMAP_HELVETICA_12);
    
    // Instruções com ícones
    glColor3f(0.6f, 0.8f, 1.0f);
    renderTextCentered(215, "Setas para esquerda e direita para mover  |  Espaco para pular  |  Seta para baixo para deslizar", GLUT_BITMAP_HELVETICA_10);
    renderTextCentered(200, "ESC para menu/pausar  |  V para câmera  |  D para debug", GLUT_BITMAP_HELVETICA_10);
    
    // Navegação
    glColor3f(1.0f, 0.8f, 0.2f);
    renderTextCentered(185, "↑↓ Navegar  |  ENTER Selecionar", GLUT_BITMAP_HELVETICA_10);
}

void Menu::renderPauseMenu() {
    // Configurar para renderização 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    // Fundo escurecido com blur effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();
    
    // Painel de pausa moderno
    glColor4f(0.05f, 0.1f, 0.2f, 0.95f);
    glBegin(GL_QUADS);
    glVertex2f(200, 200);
    glVertex2f(600, 200);
    glVertex2f(600, 450);
    glVertex2f(200, 450);
    glEnd();
    
    // Borda brilhante animada
    float time = glutGet(GLUT_ELAPSED_TIME) * 0.003f;
    float pulse = 0.5f + 0.3f * sin(time * 2.0f);
    glColor4f(1.0f, 0.6f, 0.2f, pulse);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(200, 200);
    glVertex2f(600, 200);
    glVertex2f(600, 450);
    glVertex2f(200, 450);
    glEnd();
    
    glDisable(GL_BLEND);
    
    // Título "PAUSADO" com efeito
    glColor3f(1.0f, 0.8f, 0.2f);
    renderTextCentered(400, "JOGO PAUSADO", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Opções do menu de pausa
    float optionY[] = {340, 290};
    std::string options[] = {"CONTINUAR", "MENU PRINCIPAL"};
    
    for (int i = 0; i < 2; i++) {
        bool isSelected = (selectedOption == i);
        
        if (isSelected) {
            // Fundo da opção selecionada
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            float glow = 0.6f + 0.4f * sin(time * 4.0f);
            glColor4f(1.0f, 0.6f, 0.2f, glow * 0.5f);
            glBegin(GL_QUADS);
            glVertex2f(250, optionY[i] - 15);
            glVertex2f(550, optionY[i] - 15);
            glVertex2f(550, optionY[i] + 25);
            glVertex2f(250, optionY[i] + 25);
            glEnd();
            
            glDisable(GL_BLEND);
            
            glColor3f(1.0f, 1.0f, 1.0f);
            renderTextCentered(optionY[i], "► " + options[i] + " ◄", GLUT_BITMAP_HELVETICA_18);
        } else {
            glColor3f(0.7f, 0.7f, 0.8f);
            renderTextCentered(optionY[i], options[i], GLUT_BITMAP_HELVETICA_18);
        }
    }
    
    // Instruções
    glColor3f(0.6f, 0.8f, 1.0f);
    renderTextCentered(240, "↑↓ Navegar  |  ENTER Selecionar  |  ESC Continuar", GLUT_BITMAP_HELVETICA_12);
    
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Menu::renderGameOverMenu() {
    // Implementado no Score.cpp
}

void Menu::setState(GameState state) {
    currentState = state;
    selectedOption = 0;
    
    // Configurar número de opções baseado no estado
    switch (state) {
        case MENU:
            numMenuOptions = 2;
            break;
        case PAUSED:
            numMenuOptions = 2;
            break;
        default:
            numMenuOptions = 1;
            break;
    }
}

void Menu::navigateUp() {
    selectedOption--;
    if (selectedOption < 0) {
        selectedOption = numMenuOptions - 1;
    }
}

void Menu::navigateDown() {
    selectedOption++;
    if (selectedOption >= numMenuOptions) {
        selectedOption = 0;
    }
}

void Menu::selectOption() {
    // A lógica de seleção será implementada no main.cpp
}

void Menu::renderText(float x, float y, const std::string& text, void* font) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void Menu::renderTextCentered(float y, const std::string& text, void* font) {
    int width = getTextWidth(text, font);
    float x = (800 - width) / 2.0f;
    renderText(x, y, text, font);
}

void Menu::renderTextWithShadow(float x, float y, const std::string& text, void* font) {
    // Sombra
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 1, y - 1);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
    
    // Texto principal
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void Menu::renderTextCenteredWithShadow(float y, const std::string& text, void* font) {
    int width = getTextWidth(text, font);
    float x = (800 - width) / 2.0f;
    renderTextWithShadow(x, y, text, font);
}

void Menu::renderTitle(float y, const std::string& text) {
    glColor3f(1.0f, 1.0f, 1.0f);
    renderTextCentered(y, text, GLUT_BITMAP_TIMES_ROMAN_24);
}

int Menu::getTextWidth(const std::string& text, void* font) {
    int width = 0;
    for (char c : text) {
        width += glutBitmapWidth(font, c);
    }
    return width;
}

void Menu::renderMenuOption(float y, const std::string& text, bool selected) {
    if (selected) {
        glColor3f(1.0f, 1.0f, 0.0f);
        renderTextCentered(y, "> " + text + " <");
    } else {
        glColor3f(0.7f, 0.7f, 0.8f);
        renderTextCentered(y, text);
    }
}