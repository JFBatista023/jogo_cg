#ifndef MENU_H
#define MENU_H

#include <GL/glut.h>
#include <string>

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    PAUSED
};

class Menu {
private:
    GameState currentState;
    int selectedOption;
    int numMenuOptions;
    
public:
    Menu();
    ~Menu();
    
    void render();
    void renderMainMenu();
    void renderInstructions();
    void renderGameOverMenu();
    void renderPauseMenu();
    
    // Controle de estado
    void setState(GameState state);
    GameState getState() const { return currentState; }
    
    // Navegação no menu
    void navigateUp();
    void navigateDown();
    void selectOption();
    
    // Renderização de texto
    void renderText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18);
    void renderTextCentered(float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18);
    void renderTextWithShadow(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18);
    void renderTextCenteredWithShadow(float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18);
    void renderTitle(float y, const std::string& text);
    
    // Getters
    int getSelectedOption() const { return selectedOption; }
    
    // Inicialização do estado OpenGL para o menu
    void initializeMenuState();
    
private:
    int getTextWidth(const std::string& text, void* font);
    void renderMenuOption(float y, const std::string& text, bool selected);
    
    // Novas funções para menu moderno
    void renderSpaceBackground();
    void renderStarField();
    void renderNebulas();
    void renderAnimatedParticles();
    void renderMainMenuPanel();
    void renderEnhancedTitle();
    void renderModernMenuOptions();
    void renderStylizedInstructions();
};

#endif