#ifndef SCORE_H
#define SCORE_H

#include <GL/glut.h>
#include <string>

class Score {
private:
    int currentScore;
    int highScore;
    float gameTime;
    bool gameActive;
    
public:
    Score();
    ~Score();
    
    void update(float deltaTime);
    void render();
    void renderHUD();
    void renderGameOverScreen();
    void renderMenuScreen();
    
    // Controle de pontuação
    void addScore(int points);
    void resetScore();
    void gameOver();
    void startGame();
    
    // Getters
    int getCurrentScore() const { return currentScore; }
    int getHighScore() const { return highScore; }
    float getGameTime() const { return gameTime; }
    bool isGameActive() const { return gameActive; }
    
    // Utilidades para renderização de texto
    void renderText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18);
    void renderTextCentered(float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18);
    
private:
    void loadHighScore();
    void saveHighScore();
    int getTextWidth(const std::string& text, void* font);
};

#endif 