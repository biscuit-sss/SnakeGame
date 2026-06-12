#pragma once

const int WIDTH = 15;
const int HEIGHT = 15;
const int CELL_SIZE = 30;
const int WINDOW_WIDTH = WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = HEIGHT * CELL_SIZE;

const float SNAKE_RADIUS = CELL_SIZE / 2.0f;

enum Direction { UP, DOWN, LEFT, RIGHT };
enum GameState { MENU, PLAYING, GAME_OVER, LEADERBOARD };

struct GridPos {
    int x, y;
    
    bool operator==(const GridPos& other) const {
        return x == other.x && y == other.y;
    }
};
