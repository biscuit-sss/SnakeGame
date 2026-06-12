#include "Snake.h"

void Snake::reset() {
    body.clear();
    int startX = WIDTH / 2;
    int startY = HEIGHT / 2;
    body.push_back({startX, startY});
    body.push_back({startX - 1, startY});
    body.push_back({startX - 2, startY});
    dir = RIGHT;
    nextDir = RIGHT;
}

void Snake::changeDirection(Direction newDir) {
    if (newDir == UP && dir != DOWN) nextDir = UP;
    if (newDir == DOWN && dir != UP) nextDir = DOWN;
    if (newDir == LEFT && dir != RIGHT) nextDir = LEFT;
    if (newDir == RIGHT && dir != LEFT) nextDir = RIGHT;
}

GridPos Snake::getNextHeadPos() const {
    GridPos nextHead = body.front();
    if (dir == UP)    nextHead.y -= 1;
    if (dir == DOWN)  nextHead.y += 1;
    if (dir == LEFT)  nextHead.x -= 1;
    if (dir == RIGHT) nextHead.x += 1;
    return nextHead;
}

void Snake::move(GridPos nextHead, bool ateFood) {
    body.insert(body.begin(), nextHead);
    if (!ateFood) {
        body.pop_back();
    }
}

bool Snake::checkSelfCollision(GridPos nextHead) const {
    for (const auto& part : body) {
        if (nextHead == part) return true;
    }
    return false;
}

void Snake::draw(sf::RenderWindow& window, float progress, bool isGameOver) const {
    for (int i = static_cast<int>(body.size()) - 1; i >= 0; --i) {
        float currentX = body[i].x * CELL_SIZE + CELL_SIZE / 2.0f;
        float currentY = body[i].y * CELL_SIZE + CELL_SIZE / 2.0f;

        float targetX = currentX;
        float targetY = currentY;

        if (!isGameOver) {
            if (i == 0) {
                if (dir == UP)    targetY -= CELL_SIZE;
                if (dir == DOWN)  targetY += CELL_SIZE;
                if (dir == LEFT)  targetX -= CELL_SIZE;
                if (dir == RIGHT) targetX += CELL_SIZE;
            } else {
                targetX = body[i - 1].x * CELL_SIZE + CELL_SIZE / 2.0f;
                targetY = body[i - 1].y * CELL_SIZE + CELL_SIZE / 2.0f;
            }
        }

        float smoothX = currentX + (targetX - currentX) * progress;
        float smoothY = currentY + (targetY - currentY) * progress;

        sf::CircleShape segmentShape(SNAKE_RADIUS);
        segmentShape.setFillColor((i == 0) ? sf::Color(0, 140, 0) : sf::Color(0, 190, 0));
        segmentShape.setOrigin(SNAKE_RADIUS, SNAKE_RADIUS);
        segmentShape.setPosition(smoothX, smoothY);
        
        window.draw(segmentShape);
    }
}
