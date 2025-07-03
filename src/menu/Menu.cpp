#include "Menu.h"

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
    // Configurar para renderização 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    
    // Fundo do menu MUITO MAIS CLARO
    glBegin(GL_QUADS);
    // Gradiente claro de azul para branco
    glColor3f(0.6f, 0.8f, 1.0f);  // Azul claro no topo
    glVertex2f(0, 600);
    glVertex2f(800, 600);
    glColor3f(0.8f, 0.9f, 1.0f);  // Azul muito claro no meio
    glVertex2f(800, 400);
    glVertex2f(0, 400);
    glColor3f(0.9f, 0.95f, 1.0f); // Quase branco
    glVertex2f(800, 200);
    glVertex2f(0, 200);
    glColor3f(1.0f, 1.0f, 1.0f);  // Branco puro na base
    glVertex2f(800, 0);
    glVertex2f(0, 0);
    glEnd();
    
    // Estrelas AZUIS ESCURAS para contraste no fundo claro
    glColor3f(0.0f, 0.0f, 0.8f);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 30; i++) {
        float x = (i * 73) % 800;
        float y = (i * 47 + 100) % 600;
        glVertex2f(x, y);
    }
    glEnd();
    
    // Estrelas douradas para destaque
    glColor3f(0.8f, 0.6f, 0.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 15; i++) {
        float x = (i * 127 + 50) % 800;
        float y = (i * 83 + 200) % 600;
        glVertex2f(x, y);
    }
    glEnd();
    
    // Título do jogo
    renderTitle(450, "COSMIC DASH");
    
    // Subtítulo BRANCO com sombra
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderTextCentered(401, "Endless Runner Espacial");
    glColor3f(1.0f, 1.0f, 1.0f); // texto principal branco
    renderTextCentered(400, "Endless Runner Espacial");
    
    // Opções do menu
    renderMenuOption(300, "Jogar", selectedOption == 0);
    renderMenuOption(250, "Sair", selectedOption == 1);
    
    // Instruções
    renderInstructions();
    
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Menu::renderInstructions() {
    // Título das instruções BRANCO com sombra
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderTextCentered(151, "COMO JOGAR:");
    glColor3f(1.0f, 1.0f, 1.0f); // texto principal
    renderTextCentered(150, "COMO JOGAR:");
    
    // Instruções BRANCAS com sombra
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderTextCentered(121, "Setas <- -> : Mover entre faixas");
    renderTextCentered(101, "Espaco : Pular");
    renderTextCentered(81, "ESC : Menu/Pausar");
    
    glColor3f(1.0f, 1.0f, 1.0f); // texto principal
    renderTextCentered(120, "Setas <- -> : Mover entre faixas");
    renderTextCentered(100, "Espaco : Pular");
    renderTextCentered(80, "ESC : Menu/Pausar");
    
    // Navegação AMARELA com sombra
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderTextCentered(41, "Navegacao: Setas | Enter: Selecionar");
    glColor3f(1.0f, 1.0f, 0.0f); // amarelo para destaque
    renderTextCentered(40, "Navegacao: Setas | Enter: Selecionar");
}

void Menu::renderGameOverMenu() {
    // Implementado no Score.cpp
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
    
    // Fundo semi-transparente
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();
    glDisable(GL_BLEND);
    
    // Título com sombra
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderTextCentered(401, "PAUSADO", GLUT_BITMAP_TIMES_ROMAN_24);
    renderTextCentered(399, "PAUSADO", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1.0f, 1.0f, 0.0f); // amarelo brilhante
    renderTextCentered(400, "PAUSADO", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Opções
    renderMenuOption(300, "Continuar", selectedOption == 0);
    renderMenuOption(250, "Menu Principal", selectedOption == 1);
    
    // Instruções com sombra
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderTextCentered(151, "Setas: Navegar | Enter: Selecionar");
    glColor3f(1.0f, 1.0f, 1.0f); // branco brilhante
    renderTextCentered(150, "Setas: Navegar | Enter: Selecionar");
    
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
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
    // Sombra preta
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 1, y - 1);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
    
    // Texto principal branco
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
    // Sombra preta bem marcada
    glColor3f(0.0f, 0.0f, 0.0f);
    renderTextCentered(y - 2, text, GLUT_BITMAP_TIMES_ROMAN_24);
    renderTextCentered(y + 2, text, GLUT_BITMAP_TIMES_ROMAN_24);
    renderTextCentered(y - 1, text, GLUT_BITMAP_TIMES_ROMAN_24);
    renderTextCentered(y + 1, text, GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Título principal BRANCO BRILHANTE
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
        // Fundo AZUL ESCURO para opção selecionada
        glColor3f(0.0f, 0.0f, 0.6f);
        int textWidth = getTextWidth("> " + text + " <", GLUT_BITMAP_HELVETICA_18);
        float x = (800 - textWidth) / 2.0f;
        glBegin(GL_QUADS);
        glVertex2f(x - 20, y - 8);
        glVertex2f(x + textWidth + 20, y - 8);
        glVertex2f(x + textWidth + 20, y + 20);
        glVertex2f(x - 20, y + 20);
        glEnd();
        
        // Texto selecionado AMARELO BRILHANTE no fundo azul
        glColor3f(1.0f, 1.0f, 0.0f);
        renderTextCentered(y, "> " + text + " <");
    } else {
        // Texto não selecionado BRANCO com sombra
        glColor3f(0.0f, 0.0f, 0.0f); // sombra
        renderTextCentered(y + 1, text);
        glColor3f(1.0f, 1.0f, 1.0f); // texto principal branco
        renderTextCentered(y, text);
    }
} 