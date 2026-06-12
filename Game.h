#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Snake.h"
#include "DatabaseManager.h"

class Game {
private:
    sf::RenderWindow window;
    Snake snake;
    GridPos food;
    GameState state;
    int score;

    float moveDuration;
    float moveTimer;
    sf::Clock clock;

    sf::Font font;
    sf::Text menuTitleText;
    sf::Text menuStartText;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text restartText;

    sf::Text leaderboardTitleText;
    sf::Text leaderboardBodyText;


    DatabaseManager dbManager;

    void initTexts();
    void generateFood();
    void processEvents();
    void update(float deltaTime);
    void render();
    void restartGame();

public:
    Game();
    void run();
};
