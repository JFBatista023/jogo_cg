#include "Score.h"
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

Score::Score() {
    currentScore = 0;
    highScore = 0;
    gameTime = 0.0f;
    isGameActive = false;
}

Score::~Score() {
}

void Score::startGame() {
    currentScore = 0;
    gameTime = 0.0f;
    isGameActive = true;
}

void Score::update(float deltaTime) {
    if (isGameActive) {
        gameTime += deltaTime;
        // Pontuação baseada no tempo (10 pontos por segundo)
        currentScore = (int)(gameTime * 10.0f);
    }
}

void Score::addPoints(int points) {
    if (isGameActive) {
        currentScore += points;
    }
}

void Score::gameOver() {
    isGameActive = false;
    if (currentScore > highScore) {
        highScore = currentScore;
    }
}

void Score::render() {
    if (!isGameActive) return;
    
    // Salvar estado atual do OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // Configurar para renderização 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Desabilitar teste de profundidade para HUD
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    // Habilitar blending para transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Renderizar fundo semi-transparente para a pontuação
    float hudWidth = 200.0f;
    float hudHeight = 80.0f;
    float hudX = 10.0f;
    float hudY = 600.0f - hudHeight - 10.0f; // Canto superior esquerdo
    
    // Fundo escuro semi-transparente
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(hudX, hudY);
        glVertex2f(hudX + hudWidth, hudY);
        glVertex2f(hudX + hudWidth, hudY + hudHeight);
        glVertex2f(hudX, hudY + hudHeight);
    glEnd();
    
    // Borda clara
    glColor4f(0.0f, 0.8f, 1.0f, 1.0f); // Azul ciano
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(hudX, hudY);
        glVertex2f(hudX + hudWidth, hudY);
        glVertex2f(hudX + hudWidth, hudY + hudHeight);
        glVertex2f(hudX, hudY + hudHeight);
    glEnd();
    
    // Renderizar texto da pontuação
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Branco
    
    // Pontuação atual
    std::stringstream scoreText;
    scoreText << "Pontos: " << currentScore;
    renderText(hudX + 10, hudY + 50, scoreText.str(), GLUT_BITMAP_HELVETICA_18);
    
    // Tempo de jogo
    std::stringstream timeText;
    timeText << "Tempo: " << std::fixed << std::setprecision(1) << gameTime << "s";
    renderText(hudX + 10, hudY + 25, timeText.str(), GLUT_BITMAP_HELVETICA_12);
    
    // Record
    if (highScore > 0) {
        std::stringstream recordText;
        recordText << "Record: " << highScore;
        renderText(hudX + 10, hudY + 5, recordText.str(), GLUT_BITMAP_HELVETICA_10);
    }
    
    // Restaurar matrizes
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    // Restaurar estado do OpenGL
    glPopAttrib();
}

void Score::renderGameOverScreen() {
    // Salvar estado atual do OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // Configurar para renderização 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Desabilitar teste de profundidade e iluminação
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    // Habilitar blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Overlay escuro semi-transparente sobre toda a tela
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(800, 0);
        glVertex2f(800, 600);
        glVertex2f(0, 600);
    glEnd();
    
    // Painel central para o Game Over
    float panelWidth = 400.0f;
    float panelHeight = 300.0f;
    float panelX = (800.0f - panelWidth) / 2.0f;
    float panelY = (600.0f - panelHeight) / 2.0f;
    
    // Fundo do painel - gradiente sutil
    glBegin(GL_QUADS);
        // Topo - azul escuro
        glColor4f(0.1f, 0.1f, 0.3f, 0.95f);
        glVertex2f(panelX, panelY + panelHeight);
        glVertex2f(panelX + panelWidth, panelY + panelHeight);
        // Base - azul muito escuro
        glColor4f(0.05f, 0.05f, 0.15f, 0.95f);
        glVertex2f(panelX + panelWidth, panelY);
        glVertex2f(panelX, panelY);
    glEnd();
    
    // Borda do painel
    glColor4f(0.0f, 0.8f, 1.0f, 1.0f); // Azul ciano
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(panelX, panelY);
        glVertex2f(panelX + panelWidth, panelY);
        glVertex2f(panelX + panelWidth, panelY + panelHeight);
        glVertex2f(panelX, panelY + panelHeight);
    glEnd();
    
    // Título "GAME OVER"
    glColor4f(1.0f, 0.2f, 0.2f, 1.0f); // Vermelho
    renderTextCentered(panelY + panelHeight - 50, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Linha separadora
    glColor4f(0.0f, 0.8f, 1.0f, 0.8f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(panelX + 20, panelY + panelHeight - 80);
        glVertex2f(panelX + panelWidth - 20, panelY + panelHeight - 80);
    glEnd();
    
    // Pontuação final
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Branco
    std::stringstream finalScoreText;
    finalScoreText << "Pontuacao Final: " << currentScore;
    renderTextCentered(panelY + panelHeight - 120, finalScoreText.str(), GLUT_BITMAP_HELVETICA_18);
    
    // Tempo total
    std::stringstream finalTimeText;
    finalTimeText << "Tempo Total: " << std::fixed << std::setprecision(1) << gameTime << " segundos";
    renderTextCentered(panelY + panelHeight - 150, finalTimeText.str(), GLUT_BITMAP_HELVETICA_12);
    
    // Novo record?
    if (currentScore >= highScore && currentScore > 0) {
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f); // Amarelo
        renderTextCentered(panelY + panelHeight - 180, "NOVO RECORD!", GLUT_BITMAP_HELVETICA_18);
    } else if (highScore > 0) {
        glColor4f(0.8f, 0.8f, 0.8f, 1.0f); // Cinza claro
        std::stringstream recordText;
        recordText << "Melhor Pontuacao: " << highScore;
        renderTextCentered(panelY + panelHeight - 180, recordText.str(), GLUT_BITMAP_HELVETICA_12);
    }
    
    // Instruções
    glColor4f(0.7f, 0.7f, 1.0f, 1.0f); // Azul claro
    renderTextCentered(panelY + 80, "Pressione 'R' para jogar novamente", GLUT_BITMAP_HELVETICA_12);
    renderTextCentered(panelY + 60, "Pressione 'ESC' para voltar ao menu", GLUT_BITMAP_HELVETICA_12);
    
    // Efeito de brilho no título (animação simples)
    static float glowTime = 0.0f;
    glowTime += 0.05f;
    float glowIntensity = 0.5f + 0.3f * std::sin(glowTime);
    
    glColor4f(1.0f, 0.4f, 0.4f, glowIntensity);
    renderTextCentered(panelY + panelHeight - 48, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
    
    // Restaurar matrizes
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    // Restaurar estado do OpenGL
    glPopAttrib();
}

void Score::renderText(float x, float y, const std::string& text, void* font) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void Score::renderTextCentered(float y, const std::string& text, void* font) {
    // Calcular largura do texto
    int textWidth = 0;
    for (char c : text) {
        textWidth += glutBitmapWidth(font, c);
    }
    
    // Centralizar horizontalmente
    float x = (800.0f - textWidth) / 2.0f;
    renderText(x, y, text, font);
}

int Score::getCurrentScore() const {
    return currentScore;
}

int Score::getHighScore() const {
    return highScore;
}

float Score::getGameTime() const {
    return gameTime;
}

bool Score::isActive() const {
    return isGameActive;
}