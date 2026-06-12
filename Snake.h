#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Config.h"

class Snake {
public:
    std::vector<GridPos> body;
    Direction dir;
    Direction nextDir;

    void reset();
    void changeDirection(Direction newDir);
    GridPos getNextHeadPos() const;
    void move(GridPos nextHead, bool ateFood);
    bool checkSelfCollision(GridPos nextHead) const;
    void draw(sf::RenderWindow& window, float progress, bool isGameOver) const;
};
