#include "Score.h"
#include <sstream>
#include <fstream>
#include <iostream>

Score::Score() {
    currentScore = 0;
    highScore = 0;
    gameTime = 0.0f;
    gameActive = false;
    loadHighScore();
}

Score::~Score() {
    saveHighScore();
}

void Score::update(float deltaTime) {
    if (gameActive) {
        gameTime += deltaTime;
        // Pontuação baseada no tempo (10 pontos por segundo)
        currentScore = (int)(gameTime * 10);
    }
}

void Score::render() {
    if (gameActive) {
        renderHUD();
    }
}

void Score::renderHUD() {
    // Configurar para renderização 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    
    // Fundo SÓLIDO e BRANCO para máxima visibilidade
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(10, 500);
    glVertex2f(400, 500);
    glVertex2f(400, 580);
    glVertex2f(10, 580);
    glEnd();
    
    // Borda preta grossa para contraste
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(4.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 500);
    glVertex2f(400, 500);
    glVertex2f(400, 580);
    glVertex2f(10, 580);
    glEnd();
    
    // Renderizar pontuação atual com sombra
    std::stringstream ss;
    ss << "Pontuacao: " << currentScore;
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderText(21, 559, ss.str());
    glColor3f(0.1f, 0.1f, 0.1f); // cinza escuro (melhor que preto puro)
    renderText(20, 560, ss.str());
    
    // Renderizar high score com sombra
    ss.str("");
    ss << "Recorde: " << highScore;
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderText(21, 539, ss.str());
    glColor3f(0.0f, 0.0f, 0.8f); // azul escuro
    renderText(20, 540, ss.str());
    
    // Renderizar tempo com sombra
    ss.str("");
    ss << "Tempo: " << (int)gameTime << "s";
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderText(21, 519, ss.str());
    glColor3f(0.8f, 0.0f, 0.0f); // vermelho escuro
    renderText(20, 520, ss.str());
    
    // Fundo BRANCO para instruções
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(10, 10);
    glVertex2f(790, 10);
    glVertex2f(790, 50);
    glVertex2f(10, 50);
    glEnd();
    
    // Borda preta das instruções
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 10);
    glVertex2f(790, 10);
    glVertex2f(790, 50);
    glVertex2f(10, 50);
    glEnd();
    
    // Instruções com sombra no fundo branco
    glColor3f(0.0f, 0.0f, 0.0f); // sombra
    renderText(21, 29, "Setas: Mover | Espaco: Pular | ESC: Menu");
    glColor3f(0.1f, 0.1f, 0.1f); // cinza escuro
    renderText(20, 30, "Setas: Mover | Espaco: Pular | ESC: Menu");
    
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Score::renderGameOverScreen() {
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
    
    // Título com efeito
    glColor3f(0.0f, 0.0f, 0.0f);
    renderTextCentered(398, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    renderTextCentered(402, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1.0f, 0.0f, 0.0f);
    renderTextCentered(400, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1.0f, 0.5f, 0.5f);
    renderTextCentered(401, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Pontuação final com sombra
    std::stringstream ss;
    ss << "Pontuacao Final: " << currentScore;
    glColor3f(0.0f, 0.0f, 0.0f);
    renderTextCentered(349, ss.str());
    glColor3f(0.0f, 1.0f, 1.0f);
    renderTextCentered(350, ss.str());
    
    // Novo recorde?
    if (currentScore > highScore) {
        glColor3f(0.0f, 0.0f, 0.0f);
        renderTextCentered(319, "NOVO RECORDE!");
        glColor3f(1.0f, 1.0f, 0.0f);
        renderTextCentered(320, "NOVO RECORDE!");
        glColor3f(1.0f, 0.8f, 0.0f);
        renderTextCentered(321, "NOVO RECORDE!");
    }
    
    // High score com sombra
    ss.str("");
    ss << "Recorde: " << highScore;
    glColor3f(0.0f, 0.0f, 0.0f);
    renderTextCentered(279, ss.str());
    glColor3f(1.0f, 1.0f, 0.0f);
    renderTextCentered(280, ss.str());
    
    // Instruções com sombra
    glColor3f(0.0f, 0.0f, 0.0f);
    renderTextCentered(199, "Pressione R para jogar novamente");
    renderTextCentered(169, "Pressione ESC para o menu");
    glColor3f(1.0f, 1.0f, 1.0f);
    renderTextCentered(200, "Pressione R para jogar novamente");
    renderTextCentered(170, "Pressione ESC para o menu");
    
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Score::renderMenuScreen() {
    // Implementado no Menu.cpp
}

void Score::addScore(int points) {
    currentScore += points;
}

void Score::resetScore() {
    currentScore = 0;
    gameTime = 0.0f;
}

void Score::gameOver() {
    gameActive = false;
    if (currentScore > highScore) {
        highScore = currentScore;
        saveHighScore();
    }
}

void Score::startGame() {
    gameActive = true;
    resetScore();
}

void Score::renderText(float x, float y, const std::string& text, void* font) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void Score::renderTextCentered(float y, const std::string& text, void* font) {
    int width = getTextWidth(text, font);
    float x = (800 - width) / 2.0f;
    renderText(x, y, text, font);
}

void Score::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void Score::saveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

int Score::getTextWidth(const std::string& text, void* font) {
    int width = 0;
    for (char c : text) {
        width += glutBitmapWidth(font, c);
    }
    return width;
} 