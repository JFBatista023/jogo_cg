#ifndef SCORE_H
#define SCORE_H

#include <string>

class Score {
private:
    int currentScore;
    int highScore;
    float gameTime;
    bool isGameActive;
    
    void renderText(float x, float y, const std::string& text, void* font);
    void renderTextCentered(float y, const std::string& text, void* font);
    
public:
    Score();
    ~Score();
    
    void startGame();
    void update(float deltaTime);
    void addPoints(int points);
    void gameOver();
    
    void render();
    void renderGameOverScreen();
    
    int getCurrentScore() const;
    int getHighScore() const;
    float getGameTime() const;
    bool isActive() const;
};

#endif // SCORE_H